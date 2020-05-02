#include "pokemon_battle.h"

// ALERTS FOR TIMERS
volatile bool BUTTON_ALERT = false;
volatile bool TIMER1_ALERT = true;
volatile bool TIMER3_ALERT = true;
volatile bool TIMER4_ALERT = true;
volatile bool UART0_RX_ALERT = false;
volatile bool UART0_TX_ALERT = false;


// IMAGE COORIDINATES 
volatile uint16_t POKEMON_X_ALLY = 55;
volatile uint16_t POKEMON_Y_ALLY = 220;
volatile uint16_t POKEMON_X_ENEMY = 185;
volatile uint16_t POKEMON_Y_ENEMY = 50;
volatile uint16_t CURSE_X = 50;
volatile uint16_t CURSE_Y = 60;

// TOUCH SCREEN CRAP
uint8_t touch_event;
uint16_t X_TOUCH,Y_TOUCH;

// POKEMON HEALTH
ALLY_POKEMON_HEALTH = 120;
ENEMY_POKEMON_HEALTH = 120;


//typedef struct {
//	int health;
//	int level;
  //      int moves [][];
//	int height;
//	int width;
//} Pokemon;


void move_curse(volatile PS2_DIR_t direction, volatile uint16_t *x_coord, volatile uint16_t *y_coord){
	switch(direction){
		case PS2_DIR_UP: // since top is 0 we need to subtract by 1
			*y_coord -= 1;
			break;
			
		case PS2_DIR_DOWN: // y increases as we move down so +1
			*y_coord += 1;
			break;
			
		case PS2_DIR_RIGHT: // X increases in the right direction so +1
			*x_coord += 1;
			break;
			
		case PS2_DIR_LEFT: // left corresponds to 0 so -1
			*x_coord -= 1;
			break;
		
		default: // Center and init don't require movement so don't touch x or y
			break;
	}
	
}


void battle_start(void) {
	int move;
	int i;

	//Draw the player's trainer in its original position
	lcd_draw_image(70, trainer1WidthPixels, 228,
		trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);

	// Draw the enemy trainer in its original position
	lcd_draw_image(170, redWidthPixels, 50,
		redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);

	// "Trainer Red Wants to battle"
	// Wait for button input

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
		trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_WHITE);
	lcd_draw_image(170 + move, redWidthPixels, 50,
		redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_WHITE);
	
	for (i = 0; i < 400000; i++) {
		}

	// Draw the initial states of the Pokemon
	lcd_draw_image(POKEMON_X_ENEMY, charizardWidthPixels,POKEMON_Y_ENEMY,
	charizardHeightPixels,charizardBitmaps,LCD_COLOR_WHITE, LCD_COLOR_RED);

	lcd_draw_image(POKEMON_X_ALLY, gengarWidthPixels,POKEMON_Y_ALLY,
	gengarHeightPixels,gengarBitmaps,LCD_COLOR_BLACK,LCD_COLOR_WHITE);

	for (i = 0; i < 400000; i++) {
	}

	// Draw initial healths of the Pokemon

	lcd_draw_rectangle(100, 120, 225, 15, LCD_COLOR_GREEN);
	lcd_draw_rectangle(10, 120, 50, 15, LCD_COLOR_GREEN;

	return;
}

char updateHealth(int damage, int recoil, char side) {
	if(side == 'A') {
		while(damage > 0) {
			lcd_draw_rectangle(100 + ALLY_POKEMON_HEALTH, i, 280, 15, LCD_COLOR_WHITE);
			ALLY_POKEMON_HEALTH--;
			damage--;
			for(j = 0; j < 100000; j++){
			}
			if (ALLY_POKEMON_HEALTH == 0) {
				return 'f';
			}
		}
	}

	else{
		while(damage > 0) {
			lcd_draw_rectangle(100 + ENEMY_POKEMON_HEALTH, i, 280, 15, LCD_COLOR_WHITE);
			ENEMY_POKEMON_HEALTH--;
			damage--;
			for(j = 0; j < 100000; j++){
			}
			if (ENEMY_POKEMON_HEALTH == 0) {
				return 'f';
			}
		}		
	}

	return '0';
}

void printMoveMessage(char pokemon, char move, char effect) {
	if (effect == 'r') {
		// Recharge message
	}

	else {
		switch(pokemon) {

		case 'g': 
		// Print Gengar used
		break;

		case 'a':
		// 'Ampharos used'
		break;

		case 'c':
		// 'Charizard used'
		break;

		case 'l':
		// 'Lapras used'
		break;

		default:

	}

		switch (move) {
			case '1':
			// Flamethrower
			break;

			case '2':
			// Earthquake
			break;

			case '3':
			// Fire Blast
			break;

			case '4':
			// Shadow Claw
			break;

			case '5':
			// Hydro Pump
			break;

			case '6':
			// Ice Beam
			break;

			case '7':
			// Body Slam
			break;

			case '8':
			// Sheer Cold
			break;

			case 's':
			// Shadow Ball
			break;

			case 'h':
			// Hyper Beam
			break;

			case 'y':
			// Psychic
			break;

			case 'p':
			// Protect
			break;

			case 't':
			// Thunderbolt
			break;

			case 'z':
			// Zap Cannon
			break;

			case 'd':
			// Dragon Pulse
			break;

			case 'g':
			// Power Gem
			break;

			default: 

		}

		for (i = 0; i < 5000000; i++) {
		} 

		if (effect != '0') {

			switch (effect) {
			
			case 'm':
			// But it missed
			break;

			case 's':
			// It was super effective
			break;

			case 'p':
			// Gengar protected itself
			break;

			case 'n':
			// It was not very effective
			break;

			case 'd':
			// It does not affect Gengar
			break; 

			case 'r':
			// It hurt itself with recoil
			break;

			default :

			}
		}

	}
	return;
}

void pokemon_battle_main(void){
	bool game_over = false;
	bool paused = false;
	FILE* file;
	uint16_t TIMER1_COUNT = 0;
	uint8_t* button_data;
	uint8_t eeprom_data;
	int i = 0;
	int j;
	int n = 9; // Temporary value so that nothing happens
	int r;
	int damageRecoil = 0;
	char input_char;
	char input[80];
	char moveAlly;
	char moveEnemy;
	char lastMove;
	char effectMessage1;
	char effectMessage2;
	char allyPokemon =  'g';  // Initialize to Gengar
	char enemyPokemon = 'c';  // Initialize to Charizard
	char status = '0';

	int damageD = 0;  // Damage Done
	int damageT = 0;  // Damage Taken
	int ALLY_HEALTH_MAX = 120;
	
	char start[80] = "Fight\n";
	

	// TOUCH SCREEN CRAP
//uint8_t touch_event;
//uint16_t X_TOUCH,Y_TOUCH;


	// EEPROM TESTING
		eeprom_byte_read(I2C1_BASE, ADDR_START, &eeprom_data);
		printf("EEPROM TESTING1: %i\n", eeprom_data);
		//*eeprom_data += 1; 
		eeprom_byte_write(I2C1_BASE, ADDR_START, 0x05);
		eeprom_byte_read(I2C1_BASE, ADDR_START, &eeprom_data);
		printf("EEPROM TESTING2: %i\n", eeprom_data);
			//MCP23017_GPIOB_R
	
	
	battle_start();

	while(!game_over){
			
	enableLeds(0xFF);

		// Touch sensor
		/*
		touch_event = ft6x06_read_td_status(); // FIX TOUCH SENSOR DOUBLE TOUCH FOR SOME REASON
		if(touch_event > 0){
			X_TOUCH = ft6x06_read_x();
			Y_TOUCH = ft6x06_read_y();
			printf("touch event: %i\n", touch_event);
			printf("X value: %i, Y value: %i\n",X_TOUCH,Y_TOUCH);  // just testing so far add game use later
		}
    
    gp_timer_wait(TIMER0_BASE, 5000000);
		*/
		
		
		
		// stupid button crap
		/*
		if(BUTTON_ALERT){
			button_data = 0;
			BUTTON_ALERT = false;
			 button_data = io_expander_read_reg(MCP23017_GPIOB_R);
			printf("button data: %X\n", button_data);
		}
		*/
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
		
		if(TIMER3_ALERT){
			TIMER3_ALERT = false;
			// draw a bitch
			lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 0);
		}
		//
		
		// BOX 3/4 screen bottom left	
		//lcd_draw_box(0,180,(ROWS-50), 50, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);


		//lcd_draw_image(POKEMON_X_ALLY, laprasWidthPixels,POKEMON_Y_ALLY,
		//laprasHeightPixels,laprasBitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);	

		//lcd_draw_image(120, redWidthPixels,160,
		//redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);	

		//lcd_draw_image(60, gengarWidthPixels,160,
		//gengarHeightPixels,gengarBitmaps,LCD_COLOR_WHITE,LCD_COLOR_BLACK);	

		// AMPHAROS WHAT IS WRONG WITH YOU
		//lcd_draw_image(180, ampharosWidthPixels,160,
		//ampharosHeightPixels,ampharosBitmaps,LCD_COLOR_WHITE,LCD_COLOR_YELLOW);


		//lcd_draw_string(start, 50,50, LCD_COLOR_CYAN, LCD_COLOR_BLACK);
		//enableLeds(0xFF);
		

		// All under the assumption Pokemon health is 120 long

		//n = rand() % 4; // Choose move for the enemy Pokemon
		switch(n) 
		{
			// All under the assumption Pokemon health is 120 long
			case 0:

			effectMessage2 = '0';
			if (enemyPokemon == 'c') {  // Flamethrower for Charizard
				moveEnemy = '1';
				if(allyPokemon == 'g') {
					damageT = 55;
				}
				else {
					damageT = 70;
				}
			}

			else {  // Hydro Pump for Lapras
				moveEnemy = '5';
				r = rand() % 5;

				if (r = 0) {
					effectMessage2 = 'm'; // 20% chance to miss
					damageT = 0;
				}		

				else if(allyPokemon == 'g') {
					damageT = 75;
				}

				else {
					damageT = 95;
				}
			}
	
			case 1:

			if (enemyPokemon == 'c') {  // Earthquake for Charizard
				moveEnemy = '2';
				effectMessage2 = 's';

				if(allyPokemon == 'g') {
					damageT = 80;
				}
				else {
					damageT = 110;
				}
			}

			else {  // Ice Beam
				moveEnemy = '6';
				effectMessage2 = '0';	

				if(allyPokemon == 'g') {
					damageT = 55;
				}

				else {
					damageT = 70;
				}
			}

			break;

			case 2:

			if (enemyPokemon == 'c') {  // Fire Blast for Charizard
				moveEnemy = '3';
				effectMessage2 = '0';
				r = rand() % 6;
				if (r = 0) {
					effectMessage2 = 'm'; // 15% chance to miss
					damageT = 0;
				}		

				else if(allyPokemon == 'g') {
					damageT = 80;
				}

				else {
					damageT = 100;
				}
			}

			else {  // Body Slam for Lapras
				moveEnemy = '7';
				if(allyPokemon == 'g') {
					effectMessage2 = 'd'; // Does not affect Ghost types 
					damageT = 0;
				}

				else {
					effectMessage2 = 'r';  // It hurt itself with recoil
					damageT = 80;
				}
			}
			
			break;


			case 3:

			if (enemyPokemon == 'c') {  // Thunderpunch for Charizard
				moveEnemy = '4';
				effectMessage2 = '0';

				if(allyPokemon == 'g') {
					damageT = 50;
				}

				else {
					effectMessage2 = 'n'; // Not very effective
					damageT = 30;
				}
			}

			else {  // Sheer Cold for Lapras
				moveEnemy = '8';
				r = rand() % 3;
				if (r != 2) {
					effectMessage2 = 'm'; // 66% chance to miss
					damageT = 0;
				}		

				else {
				effectMessage2 = '1';  // 1-hit KO
				damageT = 120;
				}
			}
			
			break;

			default:

		}

		// Need to implement no options for the player - actually I have a solution
		if (lastMove == 'h') {
			effectMessage1 = 'r';
			lastMove = '0';
		} 
		switch(moveAlly) 
		{
			// All under the assumption Pokemon health is 120 long
			// I know how to do crits later if we want
			case 's': // Shadow Ball for Gengar

			effectMessage1 = '0'; // No special effect
			if (enemyPokemon == 'c') {
				damageD = 65;
			}

			// Lapras damage
			else {
				damageD = 40;
			}

			break;

			
			case 'h': // Hyper Beam for Gengar

			effectMessage1 = '0';
			if (enemyPokemon == 'c') {
				damageD = 85;
			}

			else {
				damageD = 60;
			}

			break;


			case 'y': // Psychic for Gengar
			
			effectMessage1 = '0';
			if(enemyPokemon == 'c') {
				damageD = 55;
			}
			
			else {
				damageD = 35;
			}

			break;


			case 'p': // Protect for Gengar

			if (lastMove == 'p') {
				effectMessage1 = 'm';
			}

			else {
				effectMessage1 = '0';
				effectMessage2 = "p";  //  Protect message 
				damageT = 0;  // Negates damage taken
			}
			
			break;


			case 't': // Thunderbolt for Ampharos

			effectMessage1 = 's'; 
			if (enemyPokemon == 'c') {
				damageD = 100;
			}

			else {
				damageD = 60; 
			}

			break;


			case 'z': // Zap Cannon for Ampharos
			
			effectMessage1 = 's'; // Super effective
			n = rand() % 2;
			if (n == 0) {
				effectMessage1 = 'm';  // Missed
			}

			else if (enemyPokemon == 'c') {
				damageD = 120;
			}
				
			else {
				damageD = 80;
			}

			break;

			case 'd': // Dragon Pulse for Ampharos

			effectMessage1 = '0'; 
			if (enemyPokemon == 'c') {
				damageD = 40;
			}

			else {
				damageD = 25;
			}

			break;

			case 'g':  // Power Gem for Ampharos

			effectMessage1 = 's'; 
			if (enemyPokemon == 'c') {
				damageD = 110;
			}

			else { 

				damageD = 50;
			}

			break;

			default:

		}

		lastMove = moveAlly;
		if (moveEnemy == '7' && effectMessage2 != 'p') {
			damageRecoil = 10;
		}

		// MAKE THESE FUNCTION
		printMoveMessage(allyPokemon, moveAlly, effectMessage1); // E.g.: Charizard used Flamethroweer
		status = updateHealth(damageD, damageRecoil, 'A');  // A for "Ally" Pokemon

		if (status == 'f') {
			faintPokemon(allyPokemon, 'A')
		}

		printMoveMessage(enemyPokemon, moveEnemy, effectMessage2);
		status = updateHealth(damageT, damageRecoil, 'E');  // E for "Enemy" Pokemon

		if (status == 'f') {
			faintPokemon(enemyPokemon, 'E')
		}

		damageD = 0;
		damageT = 0;
		damageRecoil = 0;

	}
	
	
}