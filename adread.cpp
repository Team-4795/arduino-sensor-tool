/*
 * sensortool: a-to-d reader
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "lcdmenu.h"

extern SoftwareSerial lcdSerial;

void
adread_init()
{
	
}

void
adread_loop()
{
	int adval[4];
	uint8_t i;
	for(i = 0; i < 4; i++) {
	        adval[i] = analogRead(i);
		lcd_gotoline(i);
	        lcdSerial.print("ad");
		lcdSerial.print(i, DEC);
	        lcdSerial.print("=");
	        lcdSerial.print(adval[i], DEC);
	        lcdSerial.write(' ');
	        lcdSerial.write(' ');
	        lcdSerial.write(' ');

	        Serial.print(adval[i], DEC);
	        Serial.write("   ");
	}
        Serial.write("\r\n");
}
