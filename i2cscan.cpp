/*
 * sensortool module to probe all i2c addresses, and report those where somthing ACKs
 */

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "sensortool.h"

void
i2cscan_init()
{
	Wire.begin();
}

uint8_t
i2cscan_loop()
{
	unsigned char k = pdkeys.getkey();
	if(k == K_LEFT) {
		Serial.print("K_LEFT\n\r");
		return 1;
	}

	uint8_t addr7;
        uint8_t status;
        uint8_t count;

        count = 0;
        Serial.print("   ");
	lcdSerial.print("\xfe\x01"); // clear screen
	delay(50);
 	lcd_gotoline(1);
        for(addr7 = 1; addr7 < 127; addr7++) {
                Wire.beginTransmission(addr7);
                status = Wire.endTransmission();

                if(status) {
                        Serial.print(" --");
                } else {
                        Serial.print(" ");
                        Serial.print(addr7, HEX);
			lcdSerial.print(" ");
			lcdSerial.print(addr7, HEX);
                        count++;
                }
                if((addr7+1) % 16 == 0)
                        Serial.println();
                else if((addr7+1) % 8 == 0)
                        Serial.print(" ");

//		if((count % 6) == 0)
//			lcdSerial.print("\n");  // does that work?
		
		pdkeys.poll();
        }
        Serial.println();
	Serial.print("i2c found ");
	Serial.print(count, DEC);

 	lcd_gotoline(0);
	lcdSerial.print("i2c found ");
	lcdSerial.print(count, DEC);

	// delaying a long time, long enough for user to read the display,
	// makes for poor keypad responsiveness.  so poll it while we wait.
 	for(uint8_t i = 0; i < 20; i++) {
		pdkeys.poll();
		delay(30);
	}
	return 0;
}
