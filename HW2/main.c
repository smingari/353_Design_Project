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

//*****************************************************************************
// This is an ARRAY of strings.  If you wanted to access the 3rd string
// ("P10000"), you could do so using COMMANDS[2].
//*****************************************************************************

char * COMMANDS[] = {
    "H200000",
    "D500",
    "H10000",
    "W1000",
    "H5000",
    "A2000",
    "H2500",
    "S2000",
    "H1250",
    "D94",
    "H1",
    "W142"
};

int NUM_CMD = sizeof(COMMANDS) / sizeof(COMMANDS[0]); // get length of Command array
int delta_x = COLS/2; /// start the ship in the center of the screen
int delta_y = ROWS/2;
//*****************************************************************************
//*****************************************************************************
void init_hardware(void)
{
  initialize_serial_debug();
  lcd_config_gpio();
  lcd_config_screen();
  lcd_clear_screen(LCD_COLOR_BLACK);   
}


/******************************************************************************
* calcualte_delay(char *CMD)
* used to update the render delay between movements
*@params *CMD a pointer to the command array
*@return the int value of the numerical value in the H commands
*
******************************************************************************/
int calculate_delay(char *CMD){ 
	if(CMD[0] != 'H'){ // checks for the delay indicator in the command array
		return -1;       // if not pressent not a valid command so return -1
	}
	return atoi(&CMD[1]); // returns the int value after the H in the hold command 
}


/******************************************************************************
* get_delta(char *CMD, int *x_value, *int y_value)
*
* this method reads all other types of commands (WASD)
* based on the letter it updates the x or y variable
*
*@params *CMD a refrence to the command array
*@params *x_value a refrence to the x_value variable
*@params *y_value a refrence to the y_value variable
*@return updates either the x or y dirrection based on the leading command value
*
******************************************************************************/
void get_delta(char *CMD, int *x_value, int *y_value){
	if(CMD[0] == 'W'){ // -1 based on the fact that top left is (0,0)
		*y_value -= 1;  
	}
	else if(CMD[0] == 'S'){ // since we want to go down the screen we need to increase the y value
		*y_value += 1;
	}
	else if(CMD[0] == 'A'){ // since we want to go to the right we need to subtract from the x
		*x_value -= 1;
	}
	else if(CMD[0] == 'D'){ // since we want to go to the left we need to add to the x vvalue
		*x_value += 1;
	}	
}


/******************************************************************************
* get_pixels(char *CMD)
*
* this method reads and returns the number of pixel to move for the command
*
*@params *CMD a refrence to the command array
*@return the number of movements in the command
*
******************************************************************************/
int get_pixels(char *CMD){
	if(!(CMD[0] == 'W' || CMD[0] == 'A' || CMD[0] == 'S' || CMD[0] == 'D')){ // If not vaild command skip command
		return 0;
	}
	return atoi(&CMD[1]);
}

//*****************************************************************************
//*****************************************************************************

/******************************************************************************
* main(void)
*
* this method reads in the Command array and moves the space ship image around
* the display until the command array is done.
*
******************************************************************************/
int 
main(void)
{
  int i,j;
	int delay;
	int x_min, x_max, y_min, y_max;
	int new_delay = 0; // Preset delay to be 0 incase there is never a vaild delay given
	init_hardware();
    
  put_string("\n\r");
  put_string("******************************\n\r");
  put_string("ECE353 HW2 Fall 2018\n\r");
  put_string("Salvatore Mingari\n\r");
  put_string("******************************\n\r");    
    
	lcd_draw_image(delta_x, space_shipWidthPixels, delta_y, space_shipHeightPixels, space_shipBitmaps, LCD_COLOR_RED, LCD_COLOR_BLACK); 

	for(i = 0; i < NUM_CMD; i++){
		
		int get_delay = calculate_delay(COMMANDS[i]); // calls command to update the delay ammount 
		
		if(get_delay != -1){ // Check that valid delay command to update the delay
				new_delay = get_delay;
		}
		
		else{
			int pixels = get_pixels(COMMANDS[i]); // calls the get_pixels command to get the number of pixels to move 
			int move;
			for(move = 0; move < pixels; move++){ // begin the loop to start movement of the image
					get_delta(COMMANDS[i], &delta_x, &delta_y);
					
					// CALCULATE X AND Y
					// the image has a min and max for each direction becuase the delta value corresponds to the center of the image
					// In order to prevent the image from clipping of the display we need to know where the image fully extends to
					// by using the half the dimension size of the picture and adding it to the delta coordinate
					x_min = delta_x - (space_shipWidthPixels/2);
					x_max = delta_x + (space_shipWidthPixels/2);
					y_min = delta_y - (space_shipHeightPixels/2);
					y_max = delta_y + (space_shipHeightPixels/2);
					
					if(x_min > 0 && x_max < COLS && y_min > 0 && y_max < ROWS){
						for(j = 0; j < new_delay; j++){
							//empty loop for image render pause
						}
						// draw new image after image render
						lcd_draw_image(delta_x, space_shipWidthPixels, delta_y, space_shipHeightPixels, space_shipBitmaps, LCD_COLOR_RED, LCD_COLOR_BLACK); 
					}
					
					// Check that the image isn't of the sceen and if it is move the image in correct dirrection and stop the current command
					if(x_max == COLS){ // far right of image is off screen
						pixels = 0; // sets the movement to 0 to stop the command
						delta_x -= 1; // fix the issue that the image is off the screen
					}			
					if(x_min == 0){  // left of image is at edge off screen
						pixels = 0;
						delta_x += 1;
					}	
					if(y_max == ROWS){ // bottom of image is off screen
						pixels = 0;
						delta_y -= 1;
					}	
					if(y_min == 0){ // top of image is off screen
						pixels = 0;
						delta_y += 1;
					}	
			}				
		}
	}
		
  // Reach infinite loop
  while(1){};
}
