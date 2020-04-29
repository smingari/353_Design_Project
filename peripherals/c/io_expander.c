#include "driver_defines.h"
#include "gpio_port.h"
#include "TM4C123.h"
#include "i2c.h"
#include "io_expander.h"
#include "buttons.h"



bool io_expander_init(void){
	i2c_status_t status;
	uint8_t* data;
	
	if(!gpio_enable_port(GPIOA_BASE))
		return false;
	
	// Configure SCL 
  if(!gpio_config_digital_enable(GPIOA_BASE, PA6))
		return false;
  if(!gpio_config_alternate_function(GPIOA_BASE, PA6))
		return false;
  if(!gpio_config_port_control(GPIOA_BASE, GPIO_PCTL_PA6_M, GPIO_PCTL_PA6_I2C1SCL)) 
		return false;
    
  // Configure SDA 
  if(!gpio_config_digital_enable(GPIOA_BASE, PA7))
		return false;
  if(!gpio_config_open_drain(GPIOA_BASE, PA7))
		return false;  
  if(!gpio_config_alternate_function(GPIOA_BASE, PA7))
		return false;
  if(!gpio_config_port_control(GPIOA_BASE, GPIO_PCTL_PA7_M, GPIO_PCTL_PA7_I2C1SDA))
		return false;
	
	 //initialize i2c master to set the master address to be the I2C1 on tiva board
  if(initializeI2CMaster(I2C1_BASE)!= I2C_OK)
		return false;
	
	i2cSetSlaveAddr(I2C1_BASE, MCP23017_DEV_ID, I2C_WRITE);//set slave addr to be address of MCP23017_DEV_ID(0x27)
	 
	//FOR LEDS
	io_expander_byte_write(IO_EXPANDER_I2C_BASE, MCP23017_IOCONA_R, 0);
	io_expander_byte_write(IO_EXPANDER_I2C_BASE, MCP23017_IODIRA_R, 0);
	
	//pipe pushbuttons to sw2
	//io_expander_write_reg(MCP23017_IODIRB_R, 0x0F);//make pushbuttons to be input
	io_expander_byte_write(IO_EXPANDER_I2C_BASE, MCP23017_IOCONB_R, 0);//make pushbuttons to compare with previous value
	io_expander_byte_write(IO_EXPANDER_I2C_BASE, MCP23017_GPINTENB_R, 0x0F);
	io_expander_byte_write(IO_EXPANDER_I2C_BASE, MCP23017_GPPUB_R, 0x0F);//enable pull-up of pushbuttons
	
	//enable edge-triggering interrupts of all 4 GPIOB pushbutton ports
	if(!gpio_config_falling_edge_irq(GPIOF_BASE, PF0)) return false;
	NVIC_SetPriority(GPIOF_IRQn, 2);
	NVIC_EnableIRQ(GPIOF_IRQn);
	
	io_expander_byte_read(IO_EXPANDER_I2C_BASE, MCP23017_INTCAPB_R, data);//read the cap register in every initialization to clear interrupt
	return true;
	
}

i2c_status_t io_expander_byte_write(uint32_t i2c_base, uint8_t address, uint8_t data){
	i2c_status_t status;
	
	// Before doing anything, make sure the I2C device is idle
	while(I2CMasterBusy(i2c_base)){};
	
	//==============================================================
  // Set the I2C address to be the io_expander
  //==============================================================
	status = i2cSetSlaveAddr(i2c_base, MCP23017_DEV_ID, I2C_WRITE);
	if(status != I2C_OK)
		return status;
  
  //==============================================================
  // Send the Upper byte of the address
  //==============================================================
  status = i2cSendByte(i2c_base, address, I2C_MCS_START | I2C_MCS_RUN);	
	if(status != I2C_OK)
		return status;
	
  //==============================================================
  // Send the Byte of data to write
  //==============================================================
	status = i2cSendByte(i2c_base, data, I2C_MCS_RUN | I2C_MCS_STOP);
	if(status != I2C_OK)
		return status;

  return status;
}

i2c_status_t io_expander_byte_read(uint32_t i2c_base, uint8_t address, uint8_t* data){
	i2c_status_t status;
	
	// Before doing anything, make sure the I2C device is idle
  while ( I2CMasterBusy(i2c_base)) {};

  //==============================================================
  // Set the I2C slave address to be the I/O and in Write Mode
  //==============================================================
	status = i2cSetSlaveAddr(i2c_base, MCP23017_DEV_ID, I2C_WRITE);
	if(status != I2C_OK)
		return status;

  //==============================================================
  // Send the Upper byte of the address
  //==============================================================
	status = i2cSendByte(i2c_base, address, I2C_MCS_START | I2C_MCS_RUN);	
	if(status != I2C_OK)
		return status;

  //==============================================================
  // Set the I2C slave address to be the EEPROM and in Read Mode
  //==============================================================
	status = i2cSetSlaveAddr(i2c_base, MCP23017_DEV_ID, I2C_READ);
	if(status != I2C_OK)
		return status;

  //==============================================================
  // Read the data returned by the EEPROM
  //==============================================================
	status = i2cGetByte(i2c_base, data, I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP);
	if(status != I2C_OK)
		return status;
  
  return I2C_OK;
	
}

//*****************************************************************************
// Turns off all leds on the IO Expander
//*****************************************************************************
void disableLeds(void) {
	io_expander_byte_write(IO_EXPANDER_I2C_BASE, MCP23017_GPIOA_R, 0x00);
}


//*****************************************************************************
// Takes a 8 bit hex number that controlls the 8 LEDs on the IO Expander
// MSB coresponds with D600 on silkscreen(LED7)
//*****************************************************************************
void enableLeds(uint8_t leds) {
	io_expander_byte_write(IO_EXPANDER_I2C_BASE, MCP23017_GPIOA_R,leds);
}


/*
void debounce_fsm(void){
	
  static DEBOUNCE_STATES button_state = DEBOUNCE_ONE; SW1_BIT
  bool pin_logic_level;
  
  pin_logic_level = 
  
  switch (button_state)
  {
    case DEBOUNCE_ONE:
    {
      if(pin_logic_level)
      {
        button_state = DEBOUNCE_ONE;
      }
      else
      {
        button_state = DEBOUNCE_1ST_ZERO;
      }
      break;
    }
    case DEBOUNCE_1ST_ZERO:
    {
      if(pin_logic_level)
      {
        button_state = DEBOUNCE_ONE;
      }
      else
      {
        button_state = DEBOUNCE_2ND_ZERO;
      }
      break;
    }
    case DEBOUNCE_2ND_ZERO:
    {
      if(pin_logic_level)
      {
        button_state = DEBOUNCE_ONE;
      }
      else
      {
        button_state = DEBOUNCE_PRESSED;
      }
      break;
    }
    case DEBOUNCE_PRESSED:
    {
      if(pin_logic_level)
      {
        button_state = DEBOUNCE_ONE;
      }
      else
      {
        button_state = DEBOUNCE_PRESSED;
      }
      break;
    }
    default:
    {
      while(1){};
    }
  }
  
  if(button_state == DEBOUNCE_2ND_ZERO )
  {
    return true;
  }
  else
  {
    return false;
  }
	
	
}
*/