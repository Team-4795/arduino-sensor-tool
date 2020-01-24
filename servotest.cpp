#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <gndkeys.h>
#include <lcdvar.h>

static char signon_msg[] = "sgt servotest 12";
int blink;
SoftwareSerial lcdSerial(3, 2);  // rxpin, txpin
Keyboard pdkeys;
void handle_key(unsigned char);


typedef struct pwmunit {
	int ad;
	int min;
	int max;
} pwmunit;

pwmunit pwu[2] = {
	{ 0, 1000, 2000 },
	{ 0, 1000, 2000 } 
};



void setup() {                
	int t;

	// initialize digital pins

	pinMode(13, OUTPUT);    // PB5 LED connected on most Arduino boards:
//	pinMode(8, OUTPUT);     // PB0 protoshield LEDs can be hooked up here
	pinMode(2, OUTPUT);	// PD2 software serial for LCD
//	pinMode(9, OUTPUT);     // protoshield LEDs can be hooked up here
//	pinMode(3, OUTPUT);     // we use this hardware pwm output 
//	pinMode(10, INPUT);     // PB2 button on proto board can be plugged in here

	// pin 15 PB1/OC1A - arduino 9 - variable pwm out
	// pin 16 PB2/OC1B - arduino 10 - fixed (for now) pwm out
  
	Serial.begin(19200); 
	Serial.print(signon_msg);
	Serial.print("\n\r");

	lcdSerial.begin(9600);
	lcdSerial.print("\xfe\x01"); // clear screen
	lcdSerial.print(signon_msg);
	lcdvar_draw();


	lcdSerial.print("\xfe\x93 A");  // line 1
/*	lcdSerial.print("\xfe\xc0 BBB");  // line 2
	lcdSerial.print("\xfe\x94 CCC");  // line 3
*/
//	lcdSerial.print("\xfe\xd4----");  // line 4

	lcdSerial.print("\x7c\x8c"); // backlight 40%

	// set up 2 PWM channels on PB1 and PB2 using Timer1 
	TCCR1A = 0;     // disable all PWM on Timer1 whilst we set it up 
	ICR1 = 39999;   // TOP: frequency is every 20ms

	// Configure timer 1 for WGM=1110 (Fast PWM mode) via ICR1, prescale=8
	TCCR1A = (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1<<WGM12) | (1 << CS11);
                 
	// Set PB1 and PB2 as outputs
	DDRB |= _BV(1) |  _BV(2);
	// COM1x = 0b10: clear on match, set at BOTTOM
	TCCR1A |= 2 <<  6;  // enable PWM on port B1 in non-inverted compare mode 2
	TCCR1A |= 2 <<  4;  // enable PWM on port B2 in non-inverted compare mode 2 

	pdkeys.addhook(handle_key);

	// initialize to 1.5ms pulse  
	OCR1A = 3000; // ICR1 * 1.5 / 20;
	OCR1B = 3000; // ICR1 * 1.5 / 20;
	// other pulse widths:
	// 2000: 1ms   "0 degrees"
	// 4000: 2ms	"180 degrees"

	Serial.write('\n');
	Serial.write('\r');
	blink = 0;

	delay(300);              // wait (ms)
}


void loop() {
	extern lcdvar lcdvar_table[];

	int pwm;
	int usec;
	int range;
	int i;
	unsigned char rawkeys;
	int ad2;
 
	for(i = 0; i < 2; i++) {
		pwu[i].ad = analogRead(i); 
		Serial.print(pwu[i].ad, HEX);
		Serial.write(' ');
	}
	ad2 = analogRead(2); 
	Serial.print(ad2, HEX);
	Serial.write(' ');

	for(i = 0; i < 2; i++) {
		range = 2*(pwu[i].max - pwu[i].min); // timer1 units
		pwm = ((float)pwu[i].ad / 1024.0) * range + 2*pwu[i].min;

		if(i==1) {
			if(lcdvar_table[5].curval == 1)
				pwm = 3000;
			OCR1B = pwm;
			lcdSerial.print("\xfe\x94");  // line 3 start

		} else {
			if(lcdvar_table[2].curval == 1)
				pwm = 3000;
			OCR1A = pwm;
			lcdSerial.print("\xfe\xc0");  // line 2 start

		}
		usec = pwm/2;
		lcdSerial.print(usec, DEC);
		lcdSerial.write(' ');
		lcdSerial.write(' ');
			
		Serial.print(usec, DEC);
		Serial.write(' ');
	}

	Serial.write(' ');
	Serial.write(' ');
	Serial.write(' ');
	Serial.write(' ');
	Serial.write(' ');
	Serial.write(' ');
	Serial.write('|');
	Serial.write('\r');

	if(pdkeys.poll()) {
		pwu[0].min = lcdvar_table[0].curval;
		pwu[0].max = lcdvar_table[1].curval;
		pwu[1].min = lcdvar_table[3].curval;
		pwu[1].max = lcdvar_table[4].curval; 
	}
	lcdSerial.print("\xfe\xd4 ad2=");  // line 4 start
	lcdSerial.print(ad2, DEC);
	lcdSerial.write(' ');
	lcdSerial.write(' ');
	lcdSerial.write(' ');


	if(blink)
		digitalWrite(13, HIGH);   // set the LED on
	else
		digitalWrite(13, LOW);    // set the LED off
	delay(30);              // wait (ms)

	blink = ~blink;
}

void handle_key(unsigned char k)
{
	lcdvar_key(k);
}


//#include <WProgram.h>

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}



extern "C" void __cxa_pure_virtual() { while (1); }
