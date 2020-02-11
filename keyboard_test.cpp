/*
 * sensortool module for 
 * testing of internal keyboard, its newer API, and debouncing features.
 */

#include <Arduino.h>
#include <Wire.h>
#include "gndkeys.h"
#include "sensortool.h"

extern Keyboard pdkeys;
extern LcdMenu lcdmenu;

uint8_t outchar_count;

void keytest_setup()
{
	pdkeys.flush();

	lcdSerial.print("\xfe\x01"); // clear screen
	lcdSerial.print("keyboard test");
	lcd_goto(1, 0);
	outchar_count = 0;
}

void keytest_loop()
{
	unsigned char k = pdkeys.getkey();
	if(k) {
		Serial.print("key ");
		Serial.print(k);
		Serial.print("\n");

		if(outchar_count > 38) {
			lcd_goto(1, 0);
			lcdSerial.print("                                       ");
			lcd_goto(1, 0);
			outchar_count = 0;
		}
		lcdSerial.write('0' + k);
//		lcdSerial.write(' ');
		outchar_count += 1;
	}
}


