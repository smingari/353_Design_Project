#include "driver_defines.h"
#include "gpio_port.h"
#include "TM4C123.h"
#include "i2c.h"
#include "io_expander.h"
#include "buttons.h"

bool io_expander_init()
{
	i2c_status_t i2c_status;
	
  if(!gpio_enable_port(GPIOA_BASE))
		return false;
	
/*FOR leds*/
  // Configure SCL 
  if(!gpio_config_digital_enable(GPIOA_BASE, IO_EXPANDER_I2C_SCL_PIN))
		return false;
  if(!gpio_config_alternate_function(GPIOA_BASE, IO_EXPANDER_I2C_SCL_PIN))
		return false;
  if(!gpio_config_port_control(GPIOA_BASE, IO_EXPANDER_I2C_SCL_PCTL_M, IO_EXPANDER_I2C_SCL_PIN_PCTL)) 
		return false;
    
  // Configure SDA 
  if(!gpio_config_digital_enable(GPIOA_BASE, IO_EXPANDER_I2C_SDA_PIN))
		return false;
  if(!gpio_config_open_drain(GPIOA_BASE, IO_EXPANDER_I2C_SDA_PIN))
		return false;  
  if(!gpio_config_alternate_function(GPIOA_BASE, IO_EXPANDER_I2C_SDA_PIN))
		return false;
  if(!gpio_config_port_control(GPIOA_BASE, IO_EXPANDER_I2C_SDA_PCTL_M, IO_EXPANDER_I2C_SDA_PIN_PCTL))
		return false;

  //initialize i2c master to set the master address to be the I2C1 on tiva board
  if(initializeI2CMaster(I2C1_BASE)!= I2C_OK)
		return false;
	
	i2cSetSlaveAddr(I2C1_BASE, MCP23017_DEV_ID, I2C_WRITE);//set slave addr to be address of MCP23017_DEV_ID(0x27)
	 
	//FOR LEDS
	io_expander_write_reg(MCP23017_IOCONA_R, 0);
	io_expander_write_reg(MCP23017_IODIRA_R, 0);
	
	//pipe pushbuttons to sw2
	io_expander_write_reg(MCP23017_IODIRB_R, 0xFF);//make pushbuttons to be input
	
	io_expander_write_reg(MCP23017_IPOLB_R, 0xFF); // Polarity flips everything
	io_expander_write_reg(MCP23017_GPINTENB_R, 0xFF); // INTERRUPT ON CHANGE
	io_expander_write_reg(MCP23017_GPPUB_R, 0x0F);//enable pull-up of pushbuttons
	
	//enable edge-triggering interrupts of all 4 GPIOB pushbutton ports
	if(!gpio_config_falling_edge_irq(GPIOF_BASE, PF0)) return false;
	NVIC_SetPriority(GPIOF_IRQn, 2); // Set interrupt priority 
	NVIC_EnableIRQ(GPIOF_IRQn);
	io_expander_read_reg(MCP23017_INTCAPB_R);//read the cap register in every initialization to clear interrupt
	return true;
}


uint8_t io_expander_read_reg(uint8_t addr) {
	uint8_t data_read;
	i2c_status_t i2c_status;
	while(I2CMasterBusy(I2C1_BASE)){};
		    
	// Set slave addr to IO expander	
	i2c_status = i2cSetSlaveAddr(I2C1_BASE, MCP23017_DEV_ID, I2C_WRITE); 
	if (i2c_status != I2C_OK )
		return i2c_status;
		
	
	i2c_status = i2cSendByte(I2C1_BASE, addr, I2C_MCS_START | I2C_MCS_RUN);
	if ( i2c_status != I2C_OK )
		return i2c_status;
	
	
	i2c_status = i2cSetSlaveAddr(I2C1_BASE, MCP23017_DEV_ID, I2C_READ);
	if ( i2c_status != I2C_OK )
		return i2c_status;
	
  // read data 	
	i2c_status = i2cGetByte(I2C1_BASE, &data_read, I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP);
	if ( i2c_status != I2C_OK )
		return i2c_status;
	return data_read;
}

void io_expander_write_reg(uint8_t reg, uint8_t data) {
	i2c_status_t i2c_status;
  
  while (I2CMasterBusy(I2C1_BASE)){}
		
	// set slave addr to write
	i2cSetSlaveAddr(I2C1_BASE, MCP23017_DEV_ID, I2C_WRITE);
	
	// send start signal
	i2cSendByte(I2C1_BASE, reg, I2C_MCS_START | I2C_MCS_RUN);
	
	// send end signal  
	i2cSendByte(I2C1_BASE, data, I2C_MCS_RUN | I2C_MCS_STOP);
	
}


/***********************************
/ Turns off all LEDs on IO expander
************************************/
										
void disableLeds(void) {
	io_expander_write_reg(MCP23017_GPIOA_R, 0x00);
}

/***********************************
/ Turns on LEDs on IO expander based 
/ on hex number passed
************************************/
void enableLeds(uint8_t leds) {
	io_expander_write_reg(MCP23017_GPIOA_R, leds);
}
