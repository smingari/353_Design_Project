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
	char greeting1[80] = "\nHello there! Welcome to the world of Pokemon!";
	char greeting2[80] = "My name is Oak! People call me the pokemon Prof!\n"; 
	char greeting3[80] = "This world is inhabited by creatures called Pokemon!\n";
  char greeting4[80] = "For some people, Pokemon are pets. Others use them for fights.\n";
  char greeting5[80] = "Myself...I study Pokemon as a profession.\n";
	char paused[] = "PASUED!";
	
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

	DisableInterrupts();
	initialize_hardware();
	EnableInterrupts();
	
  pokemon_battle_main();
		
  while(1){}
}
