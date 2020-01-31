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

uint8_t i2cWrite(uint8_t reg, uint8_t val);
uint8_t i2cRead(uint8_t reg, uint8_t *val, uint16_t len);

void adps9151_setup() {
	uint8_t status;
	Wire.begin();

	status = i2cWrite(0x00,0b0110);  //enable light sensor and activate rgb mode
	status |= i2cWrite(0x04,0b01000000); //set to 16 bit resolution for 25ms response time and set measurement rate to 25ms

	lcdSerial.print("\xfe\x01"); // clear screen
	lcdSerial.print("adps9151 ");
	if(status) {
		lcdSerial.print("NOT FOUND");
	} else {
		lcdSerial.print("ok       ");
	}
}

void adps9151_loop() {
	uint8_t readBuff[12];
	uint16_t r_ir=0;
	uint16_t r_red=0;
	uint16_t r_green=0;
	uint16_t r_blue=0;
	uint16_t ir=0;

	i2cRead(0x0A,readBuff,12);

	ir=(readBuff[1]<<8)|readBuff[0];
	r_green=(readBuff[4]<<8)|readBuff[3];
	r_blue=(readBuff[7]<<8)|readBuff[6];
	r_red=(readBuff[10]<<8)|readBuff[9];

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
}


uint8_t i2cWrite(uint8_t reg, uint8_t val){
	Wire.beginTransmission(I2C_ADDR);
	Wire.write(reg);
	Wire.write(val);
	return Wire.endTransmission();
}

uint8_t i2cRead(uint8_t reg, uint8_t *val, uint16_t len){
	uint8_t status;
	Wire.beginTransmission(I2C_ADDR);
	Wire.write(reg);
	status = Wire.endTransmission();
	Wire.requestFrom(I2C_ADDR, len);
	for(uint8_t i=0; i<len; i++){
	      val[i]=Wire.read();
	}
	return status;
}

