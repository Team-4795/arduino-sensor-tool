/*
 * sensortool module to read and display 1 channel of PWM input
 * for testing a pwm-generator or robot control software 
 *  atmega328p input pins 
 *	OC1A	PB1	arduino pin digital 9   (same as the pwm output, if possible)
 
 */

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "sensortool.h"
#include "lcdvar.h"
#include "PWM.hpp"

PWM my_pwm(9); // Setup pin 9 for PWM input

void
pwmread_init()
{
 	lcdSerial.print("pwm microseconds");
	my_pwm.begin(true);  // active high pwm pulses

	Serial.print("pin=8 interrupt="); Serial.println(digitalPinToInterrupt(8));
	Serial.print("pin=9 interrupt="); Serial.println(digitalPinToInterrupt(9));
	Serial.print("pin=10 interrupt="); Serial.println(digitalPinToInterrupt(10));
	
	Serial.print("MCUCR="); Serial.println( MCUCR, HEX);
	Serial.print("SREG="); Serial.println( SREG, HEX);
	Serial.print("EIMSK="); Serial.println( EIMSK, HEX);
	Serial.print("EIFR="); Serial.println( EIFR, HEX);
	Serial.print("PCICR="); Serial.println( PCICR, HEX);
	Serial.print("PCIFR="); Serial.println( PCIFR, HEX);

	Serial.print("PCINT0="); Serial.println( PCINT0, HEX);
	Serial.print("PCINT1="); Serial.println( PCINT1, HEX);
	Serial.print("PCINT2="); Serial.println( PCINT2, HEX);
	Serial.print("PCMSK0="); Serial.println( PCMSK0, HEX);
	Serial.print("PCMSK1="); Serial.println( PCMSK1, HEX);
	Serial.print("PCMSK2="); Serial.println( PCMSK2, HEX);
	delay(10000);
}

void
pwmread_cleanup()
{
	my_pwm.end();
}

uint8_t
pwmread_loop()
{
	unsigned char k = pdkeys.getkey();

	if(k == K_ENTER) {
		pwmread_cleanup();
		return 1;
	}

	int pwmval = my_pwm.getValue();
	int pwmage = my_pwm.getAge();
	bool pin = digitalRead(9);
	
	lcdSerial.print("\xfe\xc0");  // line 2 start
 	lcdSerial.print(pwmval);
	lcdSerial.print("   \xfe\x94");  // line 3 start
	lcdSerial.print("age: ");
 	lcdSerial.print(pwmage);

	Serial.print("pin: ");
	Serial.print(pin);
	Serial.print(" pwm: ");
	Serial.print(pwmval);
	Serial.print("\tAGE: ");
	Serial.println(pwmage);

	delay(150);
	return 0;
}
