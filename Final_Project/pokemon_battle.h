#ifndef __POKEMON_BATTLE_H__
#define __POKEMON_BATTLE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "main.h"

extern volatile bool TIMER1_ALERT;

extern volatile bool TIMER4_STATUS;
extern volatile bool UART0_TX_ALERT;
extern volatile bool UART0_RX_ALERT;

void pokemon_battle_main(void);




#endif
