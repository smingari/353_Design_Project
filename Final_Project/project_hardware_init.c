// Copyright (c) 2015-19, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "project_hardware_init.h"

void initialize_hardware(void){
	
	// initialize launch pad
	lp_io_init();
	
	// initialize I2C devices
	init_serial_debug(true, true);
	eeprom_init();
	ft6x06_init();
	
	// initalize uart
	uart_init(UART0_BASE, true, true);

	// initialize peripherals 
	lcd_config_gpio();
  lcd_config_screen();
  lcd_clear_screen(LCD_COLOR_WHITE);
	io_expander_init();
  ps2_initialize();
	
	
	// Start timers
	gp_timer_config_32(TIMER1_BASE, PERIODIC, SEC_ONE, false, true); // Game Light LED
	gp_timer_config_16(TIMER4_BASE, PERIODIC, CHECK_ADC, false, true, ADC_PRESCALE);  // ADC timer
	gp_timer_config_32(TIMER3_BASE, PERIODIC, 500000, false, true);      // Cursor timer
	gp_timer_config_32(TIMER0_BASE, ONESHOT, 50000, false, false);      // Touch sensor

}


