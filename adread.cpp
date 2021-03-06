/*
 * sensortool: a-to-d reader
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "sensortool.h"

extern SoftwareSerial lcdSerial;

void
adread_init()
{
 	for(uint8_t i = 0; i < 4; i++)
		tstats[i].clear();
}

const char spinner_chars[] = {'-', '+', '|', '/', 'x'};

uint8_t
adread_loop()
{
	int adval[4];
	uint8_t i;
	static uint8_t loop_count;
	static uint8_t output_count;

	unsigned char k = pdkeys.getkey();
	if(k == K_LEFT)
		return 1;
	 if(k == K_ENTER)
	 	for(i = 0; i < 4; i++)
			tstats[i].clear();
	
	for(i = 0; i < 4; i++) {
	        adval[i] = analogRead(i);
		tstats[i].item( adval[i] );
	}

	if((loop_count++ % 30) == 0) {
		for(i = 0; i < 4; i++) {
			lcd_gotoline(i);
		        lcdSerial.print("ad");
			lcdSerial.print(i, DEC);
		        lcdSerial.print("=");
		        lcdSerial.print(adval[i], DEC);
		        lcdSerial.write(' ');

		        lcdSerial.print(tstats[i].min, DEC);
		        lcdSerial.write(' ');
//		        lcdSerial.print(stats[i].mean(), DEC);
//		        lcdSerial.write(' ');
		        lcdSerial.print(tstats[i].max, DEC);
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
	return 0;
}
