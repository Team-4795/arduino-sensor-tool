#include <Arduino.h>
//#include <SoftwareSerial.h>
#include "gndkeys.h"


Keyboard::Keyboard()
{
// possible future args: port addr, bitmask
	k_pressed = 0;
	k_new = 0;
	k_hook = NULL;

	// keypad: port D bits 7:3.  input with pullup
	DDRD &=  0x07;
	PORTD |= 0xf8;
}


void 
Keyboard::addhook(keyhook kh) {
	k_hook = kh;
}


int Keyboard::poll()
{
	unsigned char rawkeys;
	unsigned char m;
	int i;
	int res = 0;
	rawkeys = 0x1f ^ (PIND >> 3);

	if(rawkeys == k_new) {  /* stable for at least one loop */
		if(k_pressed ^ rawkeys) { /* new stable keys pressed */
			for(i = 1, m = 1; i < 6; i++, m<<=1) {
				if( (m & rawkeys) && !(m & k_pressed)) {
					// key pressed
					Serial.write('\n');
					Serial.write('p'); 
					Serial.print(i, DEC);
					Serial.write('\n');
					Serial.write('\r');
					
					if(k_hook)
						k_hook(i);
					res = i;
				}
				if( !(m & rawkeys) && (m & k_pressed)) {
					// key released
					Serial.write('\n');
					Serial.write('u');
					Serial.print(i, DEC);
					Serial.write('\n');
					Serial.write('\r');
					// todo send key-UP to user
					// or implement key-repeat
					//res = 1;
				}
			}
			k_pressed = rawkeys;
		}
	}
	k_new = rawkeys;

	return res;
}
