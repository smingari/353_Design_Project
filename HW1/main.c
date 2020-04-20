// Copyright (c) 2015-16, Joe Krachey
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

#include "main.h"


extern void hw1_search_memory(uint32_t addr); 
char DISPLAY_CMD [] = "STRT LED000FF00 HOLD1000 LED100FF00 HOLD1000 LED2FF0000 HOLD1000 LED3FF0000 HOLD1000 LED4FFFFFF HOLD1000 LED5FFFFFF HOLD1000 LED70000FF BREK";
//*****************************************************************************
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}


//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	// Initialize the UART

  init_serial_debug(false, false);

  printf("\n\r");
  printf("************************************\n\r");
  printf("ECE353 - Spring 2019 HW1\n\r");
  printf("Salvatore Mingari\n\r");
  
	// Initialize the GPIO Port D
	gpio_enable_port(GPIOD_BASE);
	gpio_config_digital_enable(GPIOD_BASE,0xFF);
	gpio_config_enable_output(GPIOD_BASE,0xFF);
  
	hw1_search_memory((uint32_t)DISPLAY_CMD);
	
  // Reach infinite loop
  while(1){};
}
