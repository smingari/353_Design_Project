#include "pokemon_battle.h"

volatile bool TIMER1_ALERT = true;
volatile bool TIMER4_ALERT = true;
volatile bool UART0_RX_ALERT = false;
volatile bool UART0_TX_ALERT = false;

volatile bool TIMER4_STATUS = true;

volatile uint16_t POKEMON_X_ALLY = 55;
volatile uint16_t POKEMON_Y_ALLY = 220;
volatile uint16_t POKEMON_X_ENEMY = 185;
volatile uint16_t POKEMON_Y_ENEMY = 50;


//typedef struct {
//	int health;
//	int level;
  //      int moves [][];
//	int height;
//	int width;
//} Pokemon;

void battle_start(void) {
	int move;
	int i;

	//Draw the player's trainer in its original position
	lcd_draw_image(70, trainer1WidthPixels, 278,
		trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);

	// Draw the enemy trainer in its original position
	lcd_draw_image(170, redWidthPixels, 50,
		redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);

	// "Trainer Red Wants to battle"

	// Move the trainers across the screen
	for(move = 0; move < 35; move++) {
		if (move > 28) {
			lcd_draw_image(70 - move, trainer4WidthPixels, 228,
				trainerHeightPixels,trainer4Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
			lcd_draw_image(170 + move, redWidthPixels, 50,
				redHeightPixels,redBitmaps,LCD_COLOR_WHITE, LCD_COLOR_RED);
		}

		else if (move > 18) 
		{
			lcd_draw_image(70 - move, trainer3WidthPixels, 228,
				trainerHeightPixels,trainer3Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
			lcd_draw_image(170 + move, redWidthPixels, 50,
				redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
		}

		else if (move > 8) {
			lcd_draw_image(70 - move, trainer2WidthPixels, 228,
				trainerHeightPixels,trainer2Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
			lcd_draw_image(170 + move, redWidthPixels, 50,
				redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
		}

		else {
			lcd_draw_image(70 - move, trainer1WidthPixels, 228,
				trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
			lcd_draw_image(170 + move, redWidthPixels, 50,
				redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
		}

		for (i = 0; i < 100000; i++) {
		}
	}

	// Clear the images of the trainers
	lcd_draw_image(70 - move, trainer1WidthPixels, 228,
		trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_WHITE;
	lcd_draw_image(170 + move, redWidthPixels, 50,
		redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_WHITE);
	
	for (i = 0; i < 400000; i++) {
		}

	// Draw the initial states of the Pokemon
	lcd_draw_image(POKEMON_X_ENEMY, charizardWidthPixels,POKEMON_Y_ENEMY,
	charizardHeightPixels,charizardBitmaps,LCD_COLOR_BLACK,LCD_COLOR_RED);

	lcd_draw_image(POKEMON_X_ALLY, gengarWidthPixels,POKEMON_Y_ALLY,
	gengarHeightPixels,gengarBitmaps,LCD_COLOR_WHITE,LCD_COLOR_BLACK);

	for (i = 0; i < 400000; i++) {
	}
	
	// Draw initial healths of the Pokemon

	lcd_draw_rectangle(100, 120, 225, 15, LCD_COLOR_GREEN2);
	lcd_draw_rectangle(10, 120, 50, 15, LCD_COLOR_GREEN2);

}
	//lcd_draw_image(30, trainer1WidthPixels,160,
	//trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);
		
	//lcd_draw_image(90, trainer2WidthPixels,160,
	//trainerHeightPixels,trainer2Bitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);

		//lcd_draw_image(150, trainer3WidthPixels,160,
		//trainerHeightPixels,trainer3Bitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);

		//lcd_draw_image(210, trainer4WidthPixels,160,
		//trainerHeightPixels,trainer4Bitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);






void pokemon_battle_main(void){
	bool game_over = false;
	bool paused = false;
	FILE* file;
	uint16_t TIMER1_COUNT = 0;
	int i = 0;
	int j;
	char input_char;
	char input[80];
	char moveAlly;
	char lastMove;
	char enemyPokemon = 'c';  // Initialize to Charizard

	int damageD = 0;  // Damage Done
	int damageT = 0;  // Damage Taken
	int ALLY_HEALTH_MAX = 120;
	
	char start[80] = "Fight\n";
	
	battle_start()

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
		
		// All under the assumption Pokemon health is 120 long
		if (lastMove == 'h') {
			// GENGAR IS RECHARGING
			lastMove = '0';
		} 
		switch(moveAlly) 
		{
			// All under the assumption Pokemon health is 120 long
			// I know how to do crits later if we want
			case 's': // Shadow Ball for Gengar

			//if (enemyPokemon = Charizard) do this much damage
			if (enemyPokemon == 'c') {
				damageD = 65;
			}

			// Lapras damage
			else {
				damageD = 40;
			}

			break;

			
			case 'h': // Hyper Beam for Gengar

			if (enemyPokemon == 'c') {
				damageD = 85;
			}

			else {
				damageD = 60;
			}

			break;


			case 'y': // Psychic for Gengar
			
			if(enemyPokemon == 'c') {
				damageD = 55;
			}
			
			else {
				damageD = 35;
			}

			break;


			case 'p': // Protect for Gengar

			if (lastMove == 'p') {
				// BUT IT MISSED
				break;
			}

			damageT = 0;  // Negates damage taken
			
			break;


			case 't': // Thunderbolt for Ampharos

			if (enemyPokemon == 'c') {
				damageD = 100;
			}

			else {
				damageD = 60; 
			}

			break;


			case 'z': // Zap Cannon for Ampharos
			// random a number 1-2
			// if (1)
			// BUT IT MISSED
			// break;
			if (enemyPokemon == 'c') {
				damageD = 120;
			}
				
			else {
				damageD = 80;
			}


			case 'd': // Dragon Pulse for Ampharos

			if (enemyPokemon == 'c') {
				damageD = 40;
			}

			else {
				damageD = 25;
			}

			case 'g':  // Power Gem for Ampharos

			if (enemyPokemon == 'c') {
				damageD = 75;
			}

			else { 
				damageD = 50;
			}

			default:

		}
		
		lastMove = moveAlly;

		// Move this to a function for updating ally health eventually
		//lcd_draw_rectangle(100, ALLY_HEALTH_MAX, 280, 15, LCD_COLOR_GREEN2);
		for(i = 0; i < damageT; i++) {
			lcd_draw_rectangle(220 - i, i, 280, 15, LCD_COLOR_WHITE);
			for(j = 0; j < 100000; j++){
			// When this is eventually moved, remember to reset damageT and damageD to 0
			}
		}
	}
	
	
}