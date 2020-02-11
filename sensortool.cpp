#include <Arduino.h>
#include <Wire.h>
#include <sensortool.h>

static char signon_msg[] = "sensortool 4";
uint8_t blink;
SoftwareSerial lcdSerial(3, 2);  // rxpin, txpin
Keyboard pdkeys;
LcdMenu lcdmenu;

extern void adread_init();
extern uint8_t adread_loop();
extern void adps9151_setup();
extern uint8_t adps9151_loop();
extern void keytest_setup();
extern uint8_t keytest_loop();

struct LcdMenu_entry sensor_menu[] = {
	{1, "1PWM Generator", 0, 0},
	{2, "2A-to-D reader", adread_init, adread_loop},
	{3, "3i2c scan", 0, 0},
	{4, "4RevColor3 ADPS9151", adps9151_setup, adps9151_loop},

	{5, "5RevColor2 TMD37821", 0, 0},
	{6, "6VL53L0X distance", 0, 0},
	{7, "7VCNL4010 distance", 0, 0},
	{8, "8keypad test", keytest_setup, keytest_loop},

	{9, "9PWM input decoder", 0, 0},
};
const int sensor_menu_N = (sizeof(sensor_menu)/sizeof(LcdMenu_entry));

void setup() {                
	int t;

	// initialize digital pins

	pinMode(13, OUTPUT);    // PB5 LED connected on most Arduino boards:
//	pinMode(8, OUTPUT);     // PB0 protoshield LEDs can be hooked up here
	pinMode(2, OUTPUT);	// PD2 software serial for LCD
//	pinMode(9, OUTPUT);     // protoshield LEDs can be hooked up here
//	pinMode(3, OUTPUT);     // we use this hardware pwm output 
//	pinMode(10, INPUT);     // PB2 button on proto board can be plugged in here

	// pin 15 PB1/OC1A - arduino 9 -  pwm out 1
	// pin 16 PB2/OC1B - arduino 10 - pwm out 2
  
	Serial.begin(19200); 
	Serial.print(signon_msg);
	Serial.print("\n\r");

	lcdSerial.begin(9600);
	lcdSerial.print("\xfe\x01"); // clear screen
	lcdSerial.print(signon_msg);
//	lcdvar_draw();

	lcdSerial.print("\xfe\x93 A");  // line 1
	Serial.write('\n');
	Serial.write('\r');
	blink = 0;

	delay(400);              // wait (ms)

	lcdmenu.init_display(sensor_menu, sensor_menu_N);
}


void loop() {
	static int menu_choice;

	pdkeys.poll();
	if(menu_choice && sensor_menu[menu_choice].loop) {
		uint8_t rc = sensor_menu[menu_choice].loop();
 		if(rc) {
			pdkeys.flush();
			//lcdmenu.display(sensor_menu, sensor_menu_N);
			lcdmenu.draw();				
			menu_choice = 0;	
		}
	} else {
		unsigned char k = pdkeys.getkey();
		menu_choice = lcdmenu.keypress(k);

		if(menu_choice) {
			Serial.print("menu_choice ");
			Serial.print(menu_choice);
			Serial.write('\n');
			Serial.write('\r');

			lcdSerial.print("\xfe\x01"); // clear screen
		        lcdSerial.print("\x7c\x8c"); // backlight 40%
			pdkeys.flush();
			delay(100);
			if(sensor_menu[menu_choice].init)
				sensor_menu[menu_choice].init();
		}
	}

 	if(blink)
 		digitalWrite(13, HIGH);   // set the LED on
 	else 
 		digitalWrite(13, LOW);    // set the LED off
	delay(30);
	blink = ~blink;
}

// subroutines here for now
uint8_t st_i2c_write_byte(uint8_t addr7, uint8_t reg, uint8_t val){
	Wire.beginTransmission(addr7);
	Wire.write(reg);
	Wire.write(val);
	return Wire.endTransmission();
}

uint8_t st_i2c_read(uint8_t addr7, uint8_t reg, uint8_t *val, uint16_t len){
	uint8_t status;
	Wire.beginTransmission(addr7);
	Wire.write(reg);
	status = Wire.endTransmission();  // should this be endTransmission(false)
	Wire.requestFrom(addr7, len);
	for(uint8_t i=0; i<len; i++){
	      val[i]=Wire.read();
	}
	return status;
}


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

extern "C" void __cxa_pure_virtual() { while (1); }
