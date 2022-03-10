/*
 * sensortool module to generate 2 channels of "servo" PWM
 *  atmega328p output pins 
 *	OC1A	PB1	arduino pin digital 9
 *	OC1B	PB2     arduino pin digital 10
 *		
 *	
 */

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "sensortool.h"
#include "lcdvar.h"

typedef struct pwmunit {
        int ad;
        int min;
        int max;
} pwmunit;

pwmunit pwu[2] = {
        { 0, 1000, 2000 },
        { 0, 1000, 2000 } 
};

void
pwmgen_init()
{
 	lcdSerial.print("pwm microseconds");

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

        // initialize to 1.5ms pulse  
        OCR1A = 3000; // ICR1 * 1.5 / 20;
        OCR1B = 3000; // ICR1 * 1.5 / 20;

	lcdvar_draw();
}

void
pwmgen_cleanup()
{
}

uint8_t
pwmgen_loop()
{
	uint8_t i;
        int pwm;
        int usec;
        int range;
        extern lcdvar lcdvar_table[];

	unsigned char k = pdkeys.getkey();
	if(k == K_ENTER) {
		pwmgen_cleanup();
		return 1;
	} else if(k) {
		lcdvar_key(k);
                pwu[0].min = lcdvar_table[0].curval;
                pwu[0].max = lcdvar_table[1].curval;
                pwu[1].min = lcdvar_table[3].curval;
                pwu[1].max = lcdvar_table[4].curval; 
        }
 
        for(i = 0; i < 2; i++) {
                pwu[i].ad = analogRead(i); 
                Serial.print(pwu[i].ad, HEX);
                Serial.write(' ');
        }
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
	return 0;
}
