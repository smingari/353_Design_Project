#ifndef __POKEMON_BATTLE_H__
#define __POKEMON_BATTLE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "main.h"
#include "ps2.h"
#include "buttons.h"

// Timer Alerts
extern volatile bool TIMER1_ALERT;   
extern volatile bool TIMER3_ALERT;
extern volatile bool TIMER4_ALERT;

// Other 
extern volatile bool UART0_TX_ALERT;
extern volatile bool UART0_RX_ALERT;
extern volatile bool BUTTON_ALERT;

// Image coordinates 
extern volatile uint16_t POKEMON_X_ALLY;
extern volatile uint16_t POKEMON_Y_ALLY; 
extern volatile uint16_t POKEMON_X_ENEMY;
extern volatile uint16_t POKEMON_Y_ENEMY;
extern volatile uint16_t CURSE_X;
extern volatile uint16_t CURSE_Y;

// Address of Eeprom data
#define ADDR_START    256

void pokemon_battle_main(void);

//*****************************************************************************
// Move the cursor based on the new direction
//*****************************************************************************
void move_curse(volatile PS2_DIR_t direction, volatile uint16_t *x_coord, volatile uint16_t *y_coord);

#endif
