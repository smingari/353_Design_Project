// Copyright (c) 2015, Joe Krachey
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

//*****************************************************************************
// Configure PA0 and PA1 to be UART pins
//*****************************************************************************
void uart0_config_gpio(void)
{
  // ADD CODE
	gpio_enable_port(GPIOA_BASE);  // enable ports
	gpio_enable_port(GPIOA_BASE);
	gpio_config_digital_enable(GPIOA_BASE,PA0);  // configure as digital pins
	gpio_config_digital_enable(GPIOA_BASE,PA1);
  gpio_config_alternate_function(GPIOA_BASE,PA0);  // configure Alternate Function
	gpio_config_alternate_function(GPIOA_BASE,PA1);
	
	// Port control pins
	gpio_config_port_control(GPIOA_BASE, GPIO_PCTL_PA0_M, GPIO_PCTL_PA0_U0RX); // Set Rx 
	gpio_config_port_control(GPIOA_BASE, GPIO_PCTL_PA1_M, GPIO_PCTL_PA1_U0TX); // set Tx
}

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
  char greeting[] = "\n\r\n\rEnter an 4-digit number: ";
  char response[] = "\n\rYou entered:";
  char exit_msg[] = "\n\r\n\rGoodbye.";
  char rx_char;
  char rx_string[5] = {0,0,0,0,0};
  uint8_t rx_count = 0;  
  
  // Configure the GPIO pins by calling uart0_config_gpio()
  uart0_config_gpio();
  
  // Configure the UART0 by calling uart_init_115K().  Disable IRQs
  uart_init(UART0_BASE, false, false);
  
  if( !validate_ice(ICE_UART_POLLING))
  {
    while(1){};
    }
  
  // Print greeting[] string above using uartTxPoll
  uart_tx_poll_string(UART0_BASE, greeting);

  for ( rx_count = 0; rx_count < 5; rx_count++)
  {
     // ADD CODE
    // User input can be recieved using the uart_rx_poll()
		rx_char = uart_rx_poll(UART0_BASE, true);

    // Echo each character using uart_tx_poll
		uart_tx_poll(UART0_BASE, rx_char);

    // Add the current char to rx_string.
		rx_string[rx_count] = rx_char;
  }

  // Print response[] using uart_tx_poll_string
	uart_tx_poll_string(UART0_BASE, response);

  // Print rx_string[] using uart_tx_poll_string
	uart_tx_poll_string(UART0_BASE, rx_string);

  // Print exit_msg[]
	uart_tx_poll_string(UART0_BASE, exit_msg);

  
  while(1)
  {
     // Infinite Loop
  }
}
