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
volatile uint16_t CURSE_X = 10;
volatile uint16_t CURSE_Y = 270;

// TOUCH SCREEN CRAP
uint8_t touch_event;
uint16_t X_TOUCH,Y_TOUCH;



bool paused = false;
	FILE* file;
	char input_char;


uint16_t TIMER1_COUNT = 0;


// POKEMON HEALTH
int ALLY_POKEMON_HEALTH = 120;
int ENEMY_POKEMON_HEALTH = 120;

char gengar_used[80] = "gengar used";
char ampharos_used[80] = "ampharos used";
char charizard_used[80] = "charizard used";
char lapras_used[80] = "lapras used";

char shadow_ball[80] = "Shadow Ball";
char hyper_beam[80] = "Hyper Beam";
char psychic[80] = "Psychic";
char protect[80] = "Protect";
	
char thunderbolt[80] = "Thunderbolt";
char zap[80] = "Zap Cannon";
char dragon[80] = "Dragon Pulse";
char gem[80] = "Power Gem";
	
char flame[80] = "Flamethrower";
char quake[80] = "Earthquake";
char blast[80] = "Fire Blast";
char white_claw[80] = "Shadow Claw";

char hydro[80] = "Hydro Pump";
char ice_ice_baby[80] = "Ice Beam";
char slam[80] = "Body Slam";
char cold[80] = "Sheer Cold";

char miss[80] = "But it missed";
char super[80] = "It was super";
char effective[80] = "effective";
char he_protect_he_attack_but_most_importantly_he_got_your_back[80] = "Gengar protected itself";
char not_effective[80] = "It was not very effective";
char no_effect[80] = "It does not affect Gengar";
char recoil[80] = "It hurt itself with recoil";

char lose[80] = "All of your pokemon have fainted";
char lose2[80] = "You blacked out";
char win[80] = "Trainer Red has been defeated";
char red_talk[80] = " ... ";
typedef struct
{
    bool up;
    bool down;
    bool left;
    bool right;
} D_Pad;




void cursor_draw(){
	if(TIMER3_ALERT){
		TIMER3_ALERT = false;
		// draw a bitch
		lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 0);
	}
}



void check_pause(){
// Interrupt alert for user input
		if(UART0_RX_ALERT){
			UART0_RX_ALERT = false;
			input_char = fgetc(file); // read user input
			
			// Check to see if ' ' has been entered for a pause of game
			if(input_char == ' '){
				printf("PAUSED!\n"); // Print out pause message and enter pause mode
				paused = true;
				
				while(paused){  // game is paused
					
					// now wait for new Input
					if(UART0_RX_ALERT){
						UART0_RX_ALERT = false;
						input_char = fgetc(file); 

						// check to se if input is vaild to reusme game
						if(input_char == ' '){ 
							paused = false;
							printf("RESUME!\n");
						}
					}
				}
			}		
		}
}

	// Touch sensor
bool check_touch(){		
		touch_event = ft6x06_read_td_status(); // FIX TOUCH SENSOR DOUBLE TOUCH FOR SOME REASON
		if(touch_event > 0){
			X_TOUCH = ft6x06_read_x();
			Y_TOUCH = ft6x06_read_y();
			printf("touch event: %i\n", touch_event);
			printf("X value: %i, Y value: %i\n",X_TOUCH,Y_TOUCH);  // just testing so far add game use later
			return true;
		}
    gp_timer_wait(TIMER0_BASE, 5000000);
		return false;
}

void blinky_boi(){
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
	}

void debounce_wait(void) 
{
  int i = 10000;
  // Delay
  while(i > 0)
  {
    i--;
  }
}


bool debounce_fsm(D_Pad* d_pad){
	static DEBOUNCE_STATES state = DEBOUNCE_ONE;
	uint8_t button_data;
	bool pin_logic_level;
	
	button_data = io_expander_read_reg(MCP23017_GPIOB_R);
	//printf("%i\n", button_data);
	if(((0x0F) & button_data) > 0)	
		pin_logic_level = true; // pressed
	else
		pin_logic_level = false; // a 0 non pressed
	
	
	  switch (state)
  {
    case DEBOUNCE_ONE:
    {
			if(pin_logic_level){
				state = DEBOUNCE_1ST_ZERO;
				//printf("go to 1st\n");
			}
			else{
				//printf("go reset\n");
				state = DEBOUNCE_ONE;
			}
      break;
    }
    case DEBOUNCE_1ST_ZERO:
    {
      if(!pin_logic_level){
				state = DEBOUNCE_ONE;
				//printf("go reset\n");
			}
			else{
				//printf("go 2nd\n");

				state = DEBOUNCE_2ND_ZERO;
			}	
      break;
    }
    case DEBOUNCE_2ND_ZERO:
    {
      if(pin_logic_level){
				state = DEBOUNCE_PRESSED;
				//printf("go final\n");
			}
			else {
				state = DEBOUNCE_ONE;
				//printf("go reset\n");
			}
      break;
    }
    case DEBOUNCE_PRESSED:
    {
      state = DEBOUNCE_ONE;
      break;
    }
    default:
    {
      while(1){};
    }
  }

	if(state == DEBOUNCE_1ST_ZERO){
		d_pad->up = button_data & 0x01;
		d_pad->down = button_data & 0x02;
		d_pad->left = button_data & 0x04;
		d_pad->right = button_data & 0x08;
		//printf("button data up: %i\n", d_pad->up);
		return true;
	}
	else{
		return false;
	}
}


void check_button(D_Pad* d_pad){

// stupid button crap
		if(BUTTON_ALERT){
			BUTTON_ALERT = false;
			debounce_fsm(d_pad);
		}

}



void move_curse(volatile PS2_DIR_t direction, volatile uint16_t *x_coord, volatile uint16_t *y_coord){
	switch(direction){
		case PS2_DIR_UP: // since top is 0 we need to subtract by 1
			lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0);
			*y_coord = 270;
			break;
			
		case PS2_DIR_DOWN: // y increases as we move down so +1
			lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0);
			*y_coord = 300;
			break;
			
		case PS2_DIR_RIGHT: // X increases in the right direction so +1
			lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0);
			*x_coord = 130;
			break;
			
		case PS2_DIR_LEFT: // left corresponds to 0 so -1
			lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0);
			*x_coord = 10;
			break;
		
		default: // Center and init don't require movement so don't touch x or y
			break;
	}
	
}


void battle_start(void) {
	int move;
	int i;
	char start[80] = "Trainer Red  Wants to Battle";
	D_Pad* d_pad = malloc(sizeof(D_Pad));
  	bool battle = true;
	
	blinky_boi();
	check_pause();
	//Draw the player's trainer in its original position
	lcd_draw_image(70, trainer1WidthPixels, 228,
		trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);

	// Draw the enemy trainer in its original position
	lcd_draw_image(170, redWidthPixels, 50,
		redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);

	// "Trainer Red Wants to battle"
	
	// Wait for button input
	lcd_draw_string(start, 40, 150, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
	while(battle){
		blinky_boi();
		check_pause();
		debounce_wait();
		check_button(d_pad);
		if(check_touch() || d_pad->down || d_pad->left || d_pad->right || d_pad->up){
				battle = false;
				lcd_draw_rectangle(0, 240, 100, 100, LCD_COLOR_WHITE);
		}
	}
	// Move the trainers across the screen
	for(move = 0; move < 35; move++) {
		blinky_boi();
		check_pause();
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
	lcd_draw_rectangle(10, 120, 50, 15, LCD_COLOR_GREEN);

	// Draw Move box
	lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
	
	// Pokemon # display with LEDs
	enableLeds(0xC3); //2 v 2
		
	return;
}

char updateHealth(int damage, int recoil, char side) {
	int j;
	
	if(side == 'E') {
		while(damage > 0) {
			lcd_draw_rectangle(10 + ENEMY_POKEMON_HEALTH, 120 - ENEMY_POKEMON_HEALTH, 50, 15, LCD_COLOR_WHITE);
			ENEMY_POKEMON_HEALTH--;
			damage--;
			for(j = 0; j < 100000; j++){
			}
			if (ENEMY_POKEMON_HEALTH == 0) {
				return 'f';
			}
		}
	}

	else{
		while(damage > 0) {
			lcd_draw_rectangle(100 + ALLY_POKEMON_HEALTH, 120 - ALLY_POKEMON_HEALTH, 225, 15, LCD_COLOR_WHITE);
			ALLY_POKEMON_HEALTH--;
			damage--;
			if(recoil > 0) {
				lcd_draw_rectangle(10 + ENEMY_POKEMON_HEALTH, 120 - ENEMY_POKEMON_HEALTH, 50, 15, LCD_COLOR_WHITE);
				ENEMY_POKEMON_HEALTH--;
				recoil--;
			}

			for(j = 0; j < 100000; j++){
			}

			if (ALLY_POKEMON_HEALTH == 0) {
				return 'f';
			}

			if(ENEMY_POKEMON_HEALTH == 0) {
				return 'g';
			}
		}	
	}
	return '0';
}

void printMoveMessage(char pokemon, char move, char effect) {	
	int i;
	
	lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
	
	
	if (effect == 'r') {
		// Recharge message
	}

	else {
		switch(pokemon) {

		case 'g': 
		// Print Gengar used
			lcd_draw_string(gengar_used, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
		break;

		case 'a':
		// 'Ampharos used'
			lcd_draw_string(ampharos_used, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
		break;

		case 'c':
		// 'Charizard used'
			lcd_draw_string(charizard_used, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
		break;

		case 'l':
		// 'Lapras used'
			lcd_draw_string(lapras_used, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
		break;

		default:
		break;
	}

		switch (move) {
			case '1':
			// Flamethrower
				lcd_draw_string(flame, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case '2':
			// Earthquake
				lcd_draw_string(quake, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case '3':
			// Fire Blast
				lcd_draw_string(blast, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case '4':
			// Shadow Claw
				lcd_draw_string(white_claw, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case '5':
			// Hydro Pump
				lcd_draw_string(hydro, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case '6':
			// Ice Beam
				lcd_draw_string(ice_ice_baby, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case '7':
			// Body Slam
				lcd_draw_string(slam, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case '8':
			// Sheer Cold
				lcd_draw_string(cold, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 's':
			// Shadow Ball
				lcd_draw_string(shadow_ball, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 'h':
			// Hyper Beam
				lcd_draw_string(hyper_beam, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 'y':
			// Psychic
				lcd_draw_string(psychic, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 'p':
			// Protect
				lcd_draw_string(protect, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 't':
			// Thunderbolt
				lcd_draw_string(thunderbolt, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 'z':
			// Zap Cannon
				lcd_draw_string(zap, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 'd':
			// Dragon Pulse
				lcd_draw_string(dragon, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 'g':
			// Power Gem
				lcd_draw_string(gem, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			default: 
			break;
		}

		for (i = 0; i < 5000000; i++) {
		} 

		if (effect != '0') {

			switch (effect) {
			
			case 'm':
			// But it missed
				lcd_draw_string(miss, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 's':
			// It was super effective 
				lcd_draw_string(super, 25, 270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
				lcd_draw_string(effective, 10, 270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 'p':
			// Gengar protected itself
				lcd_draw_string(he_protect_he_attack_but_most_importantly_he_got_your_back, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 'n':
			// It was not very effective
				lcd_draw_string(not_effective, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			case 'd':
			// It does not affect Gengar
				lcd_draw_string(no_effect, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break; 

			case 'r':
			// It hurt itself with recoil
				lcd_draw_string(recoil, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			break;

			default :
			break;
			}
		}
	}
	return;
}

void faintPokemon(char pokemon, int faints) {
	int i;
	int j;
	switch (pokemon) {
		
		// If Gengar fainted
		case 'g':
		// Slowly draw white box over Pokemon for "faint" animation
		for (i = 0; i < 80; i++) {
			lcd_draw_rectangle(0, 100, 170, i, LCD_COLOR_WHITE); 
			for (j = 0; j < 100000; j++) {}
		}

		// Wait - REPLACE WITH "Gengar has fainted".
		for (j = 0; j < 3000000; j++){}

		// "Go Ampharos!"
		lcd_draw_image(POKEMON_X_ALLY, ampharosWidthPixels,POKEMON_Y_ALLY,
		ampharosHeightPixels,ampharosBitmaps,LCD_COLOR_YELLOW, LCD_COLOR_WHITE);

		// Reset the Health Bar
		lcd_draw_rectangle(100, 120, 225, 15, LCD_COLOR_GREEN);
		ALLY_POKEMON_HEALTH = 120;
		return;


		// If Ampharos fainted
		case 'a':

		for (i = 0; i < 80; i++) {
			lcd_draw_rectangle(0, 100, 170, i, LCD_COLOR_WHITE); 
			for (j = 0; j < 100000; j++) {}
		}

		// Wait - REPLACE WITH "Ampharos has fainted".
		for (j = 0; j < 3000000; j++){}

		return;


		// If Charizard fainted
		case 'c':

		for (i = 0; i < 100; i++) {
			lcd_draw_rectangle(150, 85, 0, i, LCD_COLOR_WHITE); 
			for (j = 0; j < 100000; j++) {}
		}

		// Wait - REPLACE WITH "Charizard has fainted".
		for (j = 0; j < 3000000; j++){}

		// "Go Lapras!"
		lcd_draw_image(POKEMON_X_ENEMY, laprasWidthPixels, POKEMON_Y_ENEMY,
		laprasHeightPixels,laprasBitmaps,LCD_COLOR_WHITE, LCD_COLOR_BLUE);

		// Reset Health Bar
		lcd_draw_rectangle(10, 120, 50, 15, LCD_COLOR_GREEN);
		ENEMY_POKEMON_HEALTH = 120;

		return;

		case 'l':

		for (i = 0; i < 100; i++) {
			lcd_draw_rectangle(150, 85, 0, i, LCD_COLOR_WHITE); 
			for (j = 0; j < 100000; j++) {}
		}

		// Wait - REPLACE WITH "Charizard has fainted".
		for (j = 0; j < 3000000; j++){}


		return;

		default:
		break;

		return;
	}
}

void pokemon_battle_main(void){
	bool game_over = false;
	uint8_t* button_data;
	uint8_t eeprom_data;
	uint8_t pokemon_display = 0xC3;
	int i = 0;
	int j;
	int n = 9; // Temporary value so that nothing happens
	int r;
	int allyFaints = 0;
	int enemyFaints = 0;
	int damageRecoil = 0;
	D_Pad* d_pad = malloc(sizeof(D_Pad));
	char moveAlly;
	char moveEnemy;
	char lastMove;
	bool move_select;
	int position;
	char effectMessage1;
	char effectMessage2;
	char allyPokemon =  'g';  // Initialize to Gengar
	char enemyPokemon = 'c';  // Initialize to Charizard
	char status = '0';

	int damageD = 0;  // Damage Done
	int damageT = 0;  // Damage Taken
	int ALLY_HEALTH_MAX = 120;


	// EEPROM TESTING
	eeprom_byte_read(I2C1_BASE, ADDR_START, &eeprom_data);
	printf("Trainer Gold Win's: %i\n", eeprom_data);
		
	battle_start();
	
	while(!game_over){
		
		enableLeds(pokemon_display);
		
		debounce_wait();

	
		
		if(d_pad->up == true){
			printf("up\n");
			d_pad->up = false;
		}
		
		if(d_pad->down == true){
			printf("down\n");
			d_pad->down = false;
		}
		
		if(d_pad->left == true){
			printf("left\n");
			d_pad->left = false;
		}
		if(d_pad->right == true){
			printf("right\n");
			d_pad->right = false;
		}

		// All under the assumption Pokemon health is 120 long
		
		
		
		// Clean all text
		lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
		
		
		if (allyPokemon == 'g') {
			lcd_draw_string(shadow_ball, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(hyper_beam, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(psychic, 145,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(protect, 145,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);	
    }

    else {
			lcd_draw_string(thunderbolt, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(zap, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(dragon, 145,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(gem, 145,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
    } 

		n = rand() % 4; // Choose move for the enemy Pokemon
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

				if (r == 0) {
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
				if (r == 0) {
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
				break;

		}
		
			move_select = true;
		// Await user input
			while(move_select){		
				blinky_boi();
				check_pause();
				debounce_wait();
				cursor_draw();
				check_button(d_pad);
		
				if(CURSE_X == 10 && CURSE_Y == 270) {
            position = 1;
        }
         if (CURSE_X == 10 && CURSE_Y == 300) {
            position = 2;
        }
         if (CURSE_X == 130 && CURSE_Y == 270) {
            position = 3;
        }
         if (CURSE_X == 130 && CURSE_Y == 300) {
            position = 4;
        }
				if(d_pad->left){  // select move
					printf("select\n");
					move_select = false;
				}
			}
		
		
        // Player selects the move on the Pokemon
        if (allyPokemon == 'g' && position == 1) {
            moveAlly = 's';  // Shadow Ball for Gengar
        }

        if (allyPokemon == 'g' && position == 2) {
            moveAlly = 'h';  // Hyper Beam for Gengar
        }

        if (allyPokemon == 'g' && position == 3) {
            moveAlly = 'y';  // Psychic for Gengar
        }

        if (allyPokemon == 'g' && position == 4) {
            moveAlly = 'p';  // Protect for Gengar
        }

        if (allyPokemon == 'a' && position == 1) {
            moveAlly = 't';  // Thunderbolt for Ampharos
        }

        if (allyPokemon == 'a' && position == 2) {
            moveAlly = 'z';  // Zap Cannon for Ampharos
        }

        if (allyPokemon == 'a' && position == 3) {
            moveAlly = 'd';  // Dragon Pulse for Ampharos
        }

        if (allyPokemon == 'a' && position == 4) {
            moveAlly = 'g';  // Power Gem for Ampharos
        }
			
	
		switch(moveAlly) 
		{
			// All under the assumption Pokemon health is 120 long
			// I know how to do crits later if we want
			case 's': // Shadow Ball for Gengar
			printf("test\n");
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
				effectMessage2 = 'p';  //  Protect message 
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
			break;

		}

		if (lastMove == 'h') {
			damageD = 0;
			effectMessage1 = 'r';
		} 

		lastMove = moveAlly;
		if (moveEnemy == '7' && effectMessage2 != 'p') {
			damageRecoil = 10;
		}

		
		printMoveMessage(allyPokemon, moveAlly, effectMessage1); // E.g.: Charizard used Flamethroweer

		status = updateHealth(damageD, damageRecoil, 'E');  // Updates enemy Pokemon's health

		// Just in case player tried to use Hyper Beam twice
		if (effectMessage1 == 'r') {
			lastMove = '0';
		}

		if (status == 'f') 		{
			status = 'n';
			faintPokemon(enemyPokemon, enemyFaints);  // 'Enemy' Pokemon fainted 
			enemyFaints += 1;
			pokemon_display &= 0x83;
			if (enemyFaints == 1) {
				enemyPokemon = 'l';

			}

			else {
				// WE WIN
				pokemon_display &= 0x03;
				for(i = 0; i < 1000000; i++){}
				lcd_clear_screen(LCD_COLOR_BLACK);
				lcd_draw_string(win, 35,150, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
				lcd_draw_string(red_talk, 60,180, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
				game_over = true;
				
			  // update Eeprom Score
				eeprom_data += 1;
				eeprom_byte_write(I2C1_BASE, ADDR_START, eeprom_data);
			}
		}

		if (status != 'n') {
			printMoveMessage(enemyPokemon, moveEnemy, effectMessage2);
			status = updateHealth(damageT, damageRecoil, 'A');  // Updates our Pokemon's health
			if (status == 'f') {
				status = '0';
				faintPokemon(allyPokemon, allyFaints);  // Ally Pokemon fainted
				pokemon_display &= 0xC1; 
				allyFaints += 1;
				if (allyFaints == 1) {
					allyPokemon = 'a';
				}

				else {
					// WE LOSE
					for(i = 0; i < 1000000; i++){}
					pokemon_display &= 0xC0;
					lcd_clear_screen(LCD_COLOR_BLACK);
					lcd_draw_string(lose, 30,150, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
					lcd_draw_string(lose2, 30,170, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
					game_over = true;	
				}
			}

			else if (status == 'g') {
				status = '0';
				faintPokemon(allyPokemon, allyFaints);  // 'Enemy' Pokemon fainted 
				enemyFaints += 1;
				if (enemyFaints == 2) {
					// WE WIN
					pokemon_display &= 0x03;
					for(i = 0; i < 1000000; i++){}
					lcd_clear_screen(LCD_COLOR_BLACK);
					lcd_draw_string(win, 35,150, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
					printf("...\n");
					game_over = true;
					
					// update Eeprom Score
					eeprom_data += 1;
					eeprom_byte_write(I2C1_BASE, ADDR_START, eeprom_data);
				}
			}
		}

		damageD = 0;
		damageT = 0;
		damageRecoil = 0;
	}
	// End of game
	enableLeds(pokemon_display);
	while(1){}
}