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

const char spinner_chars[] = {'-', '+', '|', '/', 'x'};

void
adread_loop()
{
	int adval[4];
	uint8_t i;
	static uint8_t loop_count;
	static uint8_t output_count;
		
	for(i = 0; i < 4; i++) {
	        adval[i] = analogRead(i);
	}

	if((loop_count++ % 30) == 0) {
		for(i = 0; i < 4; i++) {
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
		lcd_goto(3, 19);
		lcdSerial.print( spinner_chars[ output_count & 1 ]);
		output_count++;			 	
	}
}
