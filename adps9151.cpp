/*
 * sensortool module for 
 * Analog Devices ADPS9151 color/proximity sensor
 * as found in Rev Color Sensor V3
 */

#include <Arduino.h>
#include <Wire.h>
#include "sensortool.h"

#define I2C_ADDR 0x52

uint8_t readBuff[9];
uint16_t r_ir=0;
uint16_t r_red=0;
uint16_t r_green=0;
uint16_t r_blue=0;
uint16_t ir=0;
uint16_t red=0;
uint16_t green=0;
uint16_t blue=0;

void adps9151_setup() {
	uint8_t status;
	Wire.begin();

	status = st_i2c_write_byte(I2C_ADDR, 0x00, 0b0110);  //enable light sensor and activate rgb mode
	status |= st_i2c_write_byte(I2C_ADDR, 0x04, 0b01000000); //set to 16 bit resolution for 25ms response time and set measurement rate to 25ms

	lcdSerial.print("\xfe\x01"); // clear screen
	lcdSerial.print("adps9151 ");
	if(status) {
		lcdSerial.print("NOT FOUND");
	} else {
		lcdSerial.print("ok       ");
	}
}

void adps9151_loop() {
	uint8_t buf[12];
	uint16_t r_ir=0;
	uint16_t r_red=0;
	uint16_t r_green=0;
	uint16_t r_blue=0;
	uint16_t ir=0;
	uint16_t status;

	status = st_i2c_read(I2C_ADDR, 0x0A, buf, 12); // TODO retry/re-init on fail

	ir=(buf[1]<<8)|buf[0];
	r_green=(buf[4]<<8)|buf[3];
	r_blue=(buf[7]<<8)|buf[6];
	r_red=(buf[10]<<8)|buf[9];

	Serial.print("ir=");
	Serial.print(ir,HEX);
	Serial.print(" r=");
	Serial.print(r_red,HEX);
	Serial.print(" g=");
	Serial.print(r_green,HEX);
	Serial.print(" b=");
	Serial.print(r_blue,HEX);
	Serial.print("\n");

	lcd_goto(1, 0);
	lcdSerial.print(r_red, HEX);
	lcd_goto(1, 5);
	lcdSerial.print(r_green, HEX);
	lcd_goto(1, 10);
	lcdSerial.print(r_blue, HEX);

	lcd_goto(1, 15);
	lcdSerial.print(ir, HEX);

	// TODO add stats: at least min, max.  maybe mean.
}



