#include "pokemon_battle.h"

volatile bool TIMER1_ALERT =true;
volatile bool TIMER4_ALERT = true;
volatile bool UART0_RX_ALERT = false;
volatile bool UART0_TX_ALERT = false;

volatile bool TIMER4_STATUS = true;

volatile uint16_t POKEMON_X_ALLY = COLS/2;
volatile uint16_t POKEMON_Y_ALLY = ROWS/2;
volatile uint16_t POKEMON_X_ENEMY = 50;
volatile uint16_t POKEMON_Y_ENEMY = 40;


//typedef struct {
//	int health;
//	int level;
  //      int moves [][];
//	int height;
//	int width;
//} Pokemon;








void pokemon_battle_main(void){
	bool game_over = false;
	bool paused = false;
	FILE* file;
	uint16_t TIMER1_COUNT = 0;
	int i = 0;
	int j;
	char input_char;
	char input[80];
	
	int damage = 60;
	int ALLY_HEALTH_MAX = 120;
	
	char start[80] = "Fight\n";
	
	while(!game_over){
		


		
		// Interrupt alert for user input
		if(UART0_RX_ALERT){
			UART0_RX_ALERT = false;
			input_char = fgetc(file); // read user input
			
			// Check to see if ' ' has been entered for a pause of game
			if(input_char == ' '){
				printf("PAUSED!"); // Print out pause message and enter pause mode
				paused = true;
				
				while(paused){  // game is paused
					
					// now wait for new Input
					if(UART0_RX_ALERT){
						UART0_RX_ALERT = false;
						input_char = fgetc(file); 

						// check to se if input is vaild to reusme game
						if(input_char == ' '){ 
							paused = false;
							printf("RESUME!");
						}
					}
				}
			}		
		}
		

		// Timer interrupt alert
		if(TIMER1_ALERT){
			
			TIMER1_ALERT = false; // reset the alert
			
			// code to flip the LED
			
			if (TIMER1_COUNT == 0){
				lp_io_set_pin(BLUE_BIT);
			}	
			else {
				lp_io_clear_pin(BLUE_BIT);
			}
			TIMER1_COUNT = (TIMER1_COUNT + 1) % 2;
			
		}
		//
		// BOX 3/4 screen bottom left	
		//lcd_draw_box(0,180,(ROWS-50), 50, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
	//cd_draw_image(POKEMON_X_ALLY, charizardWidthPixels,POKEMON_Y_ALLY,
	//charizardHeightPixels,charizardBitmaps,LCD_COLOR_BLACK,LCD_COLOR_RED);

		//for (i = 0; i < 1000; i++) {
		//}

		//lcd_draw_image(POKEMON_X_ALLY, laprasWidthPixels,POKEMON_Y_ALLY,
		//laprasHeightPixels,laprasBitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);
		
		//lcd_draw_image(30, trainer1WidthPixels,160,
		//trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);
		
		//lcd_draw_image(90, trainer2WidthPixels,160,
		//trainerHeightPixels,trainer2Bitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);

		//lcd_draw_image(150, trainer3WidthPixels,160,
		//trainerHeightPixels,trainer3Bitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);

		//lcd_draw_image(210, trainer4WidthPixels,160,
		//trainerHeightPixels,trainer4Bitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);	

		//lcd_draw_image(120, redWidthPixels,160,
		//redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);	

		//lcd_draw_image(60, gengarWidthPixels,160,
		//gengarHeightPixels,gengarBitmaps,LCD_COLOR_WHITE,LCD_COLOR_BLACK);	

		// AMPHAROS WHAT IS WRONG WITH YOU
		//lcd_draw_image(180, ampharosWidthPixels,160,
		//ampharosHeightPixels,ampharosBitmaps,LCD_COLOR_WHITE,LCD_COLOR_YELLOW);

		//lcd_draw_image(120, charHealthWidthPixels,160,
		//charHealthHeightPixels,charHealthBitmaps,LCD_COLOR_WHITE,LCD_COLOR_BLACK);

		//lcd_draw_image(120, amphHealthWidthPixels, 160,
		//amphHealthHeightPixels,ampharosHealthBitmaps,LCD_COLOR_WHITE, LCD_COLOR_BLACK);

		//lcd_draw_string(start, 50,50, LCD_COLOR_CYAN, LCD_COLOR_BLACK);
		//enableLeds(0xFF);
		
		lcd_draw_rectangle(100, ALLY_HEALTH_MAX, 280, 15, LCD_COLOR_GREEN2);
		for(i = 0; i < damage; i++) {
			lcd_draw_rectangle(220 - i, i, 280, 15, LCD_COLOR_WHITE);
			for(j = 0; j < 100000; j++){

			}
		}
	}
	
	
}