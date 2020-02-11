
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "gndkeys.h"
#include "lcdmenu.h"

extern SoftwareSerial lcdSerial;
extern Keyboard pdkeys;

extern uint8_t st_i2c_write_byte(uint8_t addr7, uint8_t reg, uint8_t val);
extern uint8_t st_i2c_read(uint8_t addr7, uint8_t reg, uint8_t *val, uint16_t len);

