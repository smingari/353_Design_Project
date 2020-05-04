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
	char greeting1[80] = "\nHello there! Welcome to the world of Pokemon!\n";
	char greeting2[80] = "To start the battle tap the screen or hit a directional button\n"; 
	char greeting3[80] = "To pause your adventrue hit the sapace bar.\n";
  char greeting4[80] = "When you are ready to resume your adventure press space again\n";
  char greeting5[80] = "A world of dreams and adventures with Pokemon awaits! Let's go!\n";
	
	DisableInterrupts();
	initialize_hardware();
	EnableInterrupts();
	printf("\n\r******************************\n\r");
  printf("ECE353 HW3 Spring 2020\n\r");
  printf("Salvatore Mingari, Cristian Sanchez");
  printf("\n\r");
  printf("******************************\n\r"); 
	printf(greeting1);
	printf(greeting2);
	printf(greeting3);
	printf(greeting4);
	printf(greeting5);

	
  pokemon_battle_main();
		
  while(1){}
}
