#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <stdint.h>
#include "launchpad_io.h"

// Button Debounce state machine
typedef enum 
{
  DEBOUNCE_ONE,
  DEBOUNCE_1ST_ZERO,
  DEBOUNCE_2ND_ZERO,
  DEBOUNCE_PRESSED,
	DEBOUNCE_DONE
} DEBOUNCE_STATES;


bool button_pressed(uint8_t);

#endif