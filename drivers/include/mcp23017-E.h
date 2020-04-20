#ifndef __MCP23017_H__
#define __MCP23017_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "driver_defines.h"
#include "gpio_port.h"
#include "i2c.h"


extern volatile bool checkButton;

bool io_expander_init(void); 

void enableLeds(void);

void disableLeds(void);

i2c_status_t io_expander_byte_write(uint32_t i2c_base, uint16_t addr, uint8_t data);

i2c_status_t io_expander_byte_read(uint32_t i2c_base, uint16_t addr, uint8_t* data);

uint8_t detect_button_press(void);