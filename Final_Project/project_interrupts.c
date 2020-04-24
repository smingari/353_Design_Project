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

#include "main.h"
#include "project_interrupts.h"

static volatile uint16_t PS2_X_DATA = 0;
static volatile uint16_t PS2_Y_DATA = 0;
static volatile bool TIMER1_STATUS = false;



//*****************************************************************************
// Returns the most current direction that was pressed.
//*****************************************************************************
PS2_DIR_t ps2_get_direction(void)
{
  // Based on the HW3 requirements for joystick movement
	// All threshold values are given as macros
	
	// Will choose the X movement over Y since first two if statments are in X direction 
	if(PS2_X_DATA > PS2_ADC_HIGH_THRESHOLD){   // High ADC voltage corresponds with LEFT movement 
		return PS2_DIR_LEFT;
	}
	else if(PS2_X_DATA < PS2_ADC_LOW_THRESHOLD ){  // Low ADC voltage corresponds with RIGHT movement 
		return PS2_DIR_RIGHT;
	}
	else if(PS2_Y_DATA > PS2_ADC_HIGH_THRESHOLD){ // High ADC voltage corresponds with UP direction
		return PS2_DIR_UP;
	}
	else if(PS2_Y_DATA < PS2_ADC_LOW_THRESHOLD){  // Low ADC voltage corresponds with DOWN direction
		return PS2_DIR_DOWN;
	}	
	else{
		return PS2_DIR_CENTER;    // Since the joystick doesn't meet any of the thresholds we know it is in the center
	}
}


void TIMER1A_Handler(void){
	
	// Clear the interrupt
	TIMER1->ICR |= TIMER_ICR_TATOCINT;


}

//*****************************************************************************
// ADC0 SS2 ISR
//*****************************************************************************
void ADC0SS2_Handler(void)
{
	PS2_X_DATA = ADC0->SSFIFO2; // Read Sequence Sampler 2 FIFO for X & Y data
  PS2_Y_DATA = ADC0->SSFIFO2;
	//PS2_DIR = ps2_get_direction(); // Call get_direction to update direction based on joystick movement
	
  // Clear the interrupt
  ADC0->ISC |= ADC_ISC_IN2;
}



