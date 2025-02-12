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

// Used for user Input 
bool paused = false;
FILE* file;
char input_char;

// Used for the Blink-182 timer
uint16_t TIMER1_COUNT = 0;


// POKEMON HEALTH
int ALLY_POKEMON_HEALTH = 120;
int ENEMY_POKEMON_HEALTH = 120;

// String that are used are offten 
char gengar[20] = "GENGAR";
char ampharos[20] = "AMPHAROS";
char charizard[20] = "CHARIZARD";
char lapras[20] = "LAPRAS";

char gengar_used[80] = "Gengar used";
char ampharos_used[80] = "Ampharos used";
char charizard_used[80] = "Charizard used";
char lapras_used[80] = "Lapras used";
char gengar_recharge[80] = "Gengar is recharging";

// Move List
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

// Attack messages
char miss[80] = "But it missed";
char super[80] = "It was super effective";
char he_protect_he_attack_but_most_importantly_he_got_your_back[80] = "Gengar protected itself";
char not_effective[80] = "It was not very effective";
char no_effect[80] = "It does not affect Gengar";
char recoil[80] = "It hurt itself with recoil";

// Fainted pokemon message ... RIP cubone's mom :(
char gengarFaint[80] = "Gengar has fainted";
char amphFaint[80] = "Ampharos has fainted";
char charFaint[80] = "Charizard has fainted";
char laprasFaint[80] = "Lapras has fainted";
char lose[80] = "All of your pokemon have fainted";
char lose2[80] = "You blacked out";
char win[80] = "Trainer Red has been defeated";
char red_talk[80] = " ... ";

// Structue for button input
typedef struct
{
    bool up;
    bool down;
    bool left;
    bool right;
} D_Pad;



/*********************************************************
/ This method checks for the TIMER3 Alert which signals 
/ the updating of the cursor image
*********************************************************/
void cursor_draw(){
	if(TIMER3_ALERT){
		TIMER3_ALERT = false;
		lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 0);
	}
}


/*********************************************************
/ This method checks for the UART Alert from interrupt handler 
/ that signals the updating of user input into the serial debugger
*********************************************************/
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

/*********************************************************
/ This method is used when waiting for user input on the 
/ touch screen. It calls the ft6x06 that read the status 
/ register
*********************************************************/
bool check_touch(){		
	touch_event = ft6x06_read_td_status(); // Read status to detect touch
	if(touch_event > 0){
		return true;
	}
    gp_timer_wait(TIMER0_BASE, 5000000); // wait so we aren't flooded with touch
		return false;
}


/*********************************************************
/ This method recieves the signal sent from the TIMER1
/ interrupt handler and toggles the lp gpio LED on and off
*********************************************************/
void blinky_boi(){
	// Timer interrupt alert
	if(TIMER1_ALERT){			
		TIMER1_ALERT = false; // reset the alert
			
		// Code to flip the LED	
		if (TIMER1_COUNT == 0){
			lp_io_set_pin(BLUE_BIT);
		}	
		else {
			lp_io_clear_pin(BLUE_BIT);
		}
		TIMER1_COUNT = (TIMER1_COUNT + 1) % 2;
			
	}
}


/*********************************************************
/ Used to implement the debounce FSM 
*********************************************************/
void debounce_wait(void) 
{
  int i = 10000;
  // Delay
  while(i > 0)
  {
    i--;
  }
}


/*********************************************************
/ This method is used to debounce the push buttons from
/ I/O expander 
*********************************************************/
bool debounce_fsm(D_Pad* d_pad){
	static DEBOUNCE_STATES state = DEBOUNCE_ONE;
	uint8_t button_data;
	bool pin_logic_level;
	
	button_data = io_expander_read_reg(MCP23017_GPIOB_R); // Read the GPIOB port on I/O expander
	if(((0x0F) & button_data) > 0)	// Push button data detected
		pin_logic_level = true; // pressed
	else
		pin_logic_level = false; // a 0 non pressed
	
	
	// State machine for the debounce 
	switch (state) {

    case DEBOUNCE_ONE:
    {
			if(pin_logic_level){
				state = DEBOUNCE_1ST_ZERO;
			}
			else{
				state = DEBOUNCE_ONE;
			}
      break;
    }
    case DEBOUNCE_1ST_ZERO:
    {
      if(!pin_logic_level){
				state = DEBOUNCE_ONE;
			}
			else{
				state = DEBOUNCE_2ND_ZERO;
			}	
      break;
    }
    case DEBOUNCE_2ND_ZERO:
    {
      if(pin_logic_level){
				state = DEBOUNCE_PRESSED;
			}
			else {
				state = DEBOUNCE_ONE;
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

	// Beautiful masking logic on the push buttons
	if(state == DEBOUNCE_1ST_ZERO){
		d_pad->up = button_data & 0x01;
		d_pad->down = button_data & 0x02;
		d_pad->left = button_data & 0x04;
		d_pad->right = button_data & 0x08;
		return true;
	}

	else {
		return false;
	}
}

/*********************************************************
/ This method recieves the interrupt handler signal from
/ from GPIO_F handler 
*********************************************************/
void check_button(D_Pad* d_pad){
		// Buttons
		if(BUTTON_ALERT){
			BUTTON_ALERT = false;
			debounce_fsm(d_pad);  // Call debounce on the buttons
		}
}


/*********************************************************
/ This method updates the cordinates of the cursor that
/ the user controls with the PS2 joystick
*********************************************************/
void move_curse(volatile PS2_DIR_t direction, volatile uint16_t *x_coord, volatile uint16_t *y_coord){
	switch(direction){
		case PS2_DIR_UP: 
			lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0); // white out old box
			*y_coord = 270; // only can go up so set to the top row
			break;
			
		case PS2_DIR_DOWN: 
			lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0); // white out old box
			*y_coord = 300; // can only go down to set to bottom row
			break;
			
		case PS2_DIR_RIGHT:
			lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0); // white out old box
			*x_coord = 130; // can only go right so shift to far right coloum 
			break;
			
		case PS2_DIR_LEFT: 
			lcd_draw_box(CURSE_X,5,CURSE_Y,5, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0); // white out old box
			*x_coord = 10; // // can only go left so shift to far left coloum 
			break;
		
		default: // Center and init don't require movement so don't touch x or y
			break;
	}
	
}

// Function to initialize the battle
void battle_start(void) {
	int move;
	int i;
	char start[80] = "Trainer Red  Wants to Battle"; // Opening message
	D_Pad* d_pad = malloc(sizeof(D_Pad));
  bool battle = true;
	char* str = "Trainer Gold  Wins: "; // Keeps track of player's wins
	char wins[80];
	char score[80];
	uint8_t eeprom_data;
	
	
	
	blinky_boi(); // Blinking LED
	check_pause();
	//Draw the player's trainer in its original position
	lcd_draw_image(70, trainer1WidthPixels, 228,
		trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);

	// Draw the enemy trainer in its original position
	lcd_draw_image(170, redWidthPixels, 50,
		redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);

	// Keeps score of player's wins
	eeprom_byte_read(I2C1_BASE, ADDR_START, &eeprom_data);
	
	
	sprintf(wins,"%d", eeprom_data);
	
	strcpy(score, str);
	strcat(score, wins);
		
	
	lcd_draw_string(score, 40,170, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
	lcd_draw_string(start, 40, 150, LCD_COLOR_BLACK, LCD_COLOR_WHITE);

	// Wait for button input
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

		// Our Trainer's animation
		if (move > 28) {  // Part 4 of animation
			lcd_draw_image(70 - move, trainer4WidthPixels, 228,
				trainerHeightPixels,trainer4Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
			lcd_draw_image(170 + move, redWidthPixels, 50,
				redHeightPixels,redBitmaps,LCD_COLOR_WHITE, LCD_COLOR_RED);
		}

		else if (move > 18) // Part 3 of animation
		{
			lcd_draw_image(70 - move, trainer3WidthPixels, 228,
				trainerHeightPixels,trainer3Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
			lcd_draw_image(170 + move, redWidthPixels, 50,
				redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
		}

		else if (move > 8) {  // Part 2 of animation
			lcd_draw_image(70 - move, trainer2WidthPixels, 228,
				trainerHeightPixels,trainer2Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
			lcd_draw_image(170 + move, redWidthPixels, 50,
				redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
		}

		else {  // Part 1 of animation
			lcd_draw_image(70 - move, trainer1WidthPixels, 228,
				trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
			lcd_draw_image(170 + move, redWidthPixels, 50,
				redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_RED);
		}

		// Arbitrary delay to slow the game down
		for (i = 0; i < 100000; i++) {
		}
	}

	// Clear the images of the trainers
	lcd_draw_image(70 - move, trainer1WidthPixels, 228,
		trainerHeightPixels,trainer1Bitmaps,LCD_COLOR_WHITE,LCD_COLOR_WHITE);
	lcd_draw_image(170 + move, redWidthPixels, 50,
		redHeightPixels,redBitmaps,LCD_COLOR_WHITE,LCD_COLOR_WHITE);
	
	// Delay
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

	// Draw Pokemon Names
	lcd_draw_string(gengar, 150, 210, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
	lcd_draw_string(charizard, 15, 30, LCD_COLOR_BLACK, LCD_COLOR_WHITE);

	// Draw Move Box
	lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
	
	// Pokemon # display with LEDs
	enableLeds(0xC3); //2 v 2
		
	return;
}

// Function that updates health bars of Pokemon.  Also checks for faints
char updateHealth(int damage, int recoil, char side) {
	int j;
	blinky_boi();  // Blinking LED
	check_pause();
	// If the ENEMY is taking damage
	if(side == 'E') {
		while(damage > 0) {
			blinky_boi();
			check_pause();
			lcd_draw_rectangle(10 + ENEMY_POKEMON_HEALTH, 120 - ENEMY_POKEMON_HEALTH, 50, 15, LCD_COLOR_WHITE);
			ENEMY_POKEMON_HEALTH--;
			damage--;
			for(j = 0; j < 100000; j++){
			}

			// Check if Pokemon fainted
			if (ENEMY_POKEMON_HEALTH == 0) {
				return 'f'; 
			}
		}
	}

	// If WE are taking damage
	else{
		while(damage > 0) {
			blinky_boi();
			check_pause();
			lcd_draw_rectangle(100 + ALLY_POKEMON_HEALTH, 120 - ALLY_POKEMON_HEALTH, 225, 15, LCD_COLOR_WHITE);
			ALLY_POKEMON_HEALTH--;
			damage--;

			// Check for Body Slam on Lapras causing recoil damage
			if(recoil > 0) {
				lcd_draw_rectangle(10 + ENEMY_POKEMON_HEALTH, 120 - ENEMY_POKEMON_HEALTH, 50, 15, LCD_COLOR_WHITE);
				ENEMY_POKEMON_HEALTH--;
				recoil--;
			}

			for(j = 0; j < 100000; j++){
			}

			// Check if our Pokemon fainted
			if (ALLY_POKEMON_HEALTH == 0) {
				return 'f';
			}

			// Check if recoil caused Lapras to faint
			if(ENEMY_POKEMON_HEALTH == 0) {
				return 'g';
			}
		}	
	}
	return '0';
}

// Function that prints the move each Pokemon uses in the text box
// Also prints the effect of said move
// Format Example: "Gengar used _____" , "It's super effective"
void printMoveMessage(char pokemon, char move, char effect) {	
	int i;
	lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
	blinky_boi();  // Blinking LED
	check_pause();
	
	// If Gengar is recharging
	if (effect == 'r') {
		// Recharge message
		lcd_draw_string(gengar_recharge, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
	}

	else {
		// Prints 'Pokemon used'
		switch(pokemon) {

		case 'g': 
			// Print 'Gengar used'
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

	// Prints out the move
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

		// Arbitrary delay to read text
		for (i = 0; i < 5000000; i++) {
		} 

		// Checks if there is an effect message
		if (effect != '0') {
			blinky_boi();
			check_pause();
			lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);

			// Prints out the effect message
			switch (effect) {
			
			case 'm':
				// But it missed
				lcd_draw_string(miss, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
				break;

			case 's':
				// It was super effective 
				lcd_draw_string(super, 25, 270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
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

			case 'u':
				// It hurt itself with recoil
				lcd_draw_string(recoil, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
				break;

			default :
				break;
			}

			for (i = 0; i < 5000000; i++) {}
			
		}
	}
	return;
}

// Function to animate the death of each Pokemon
// Also initializes the state of the next Pokemon if possible
void faintPokemon(char pokemon, int faints) {
	int i;
	int j;
	blinky_boi();  // Blinking LED
	check_pause();

	// Check which Pokemon fainted
	switch (pokemon) {
		
		// If Gengar fainted
		case 'g':
			// Slowly draw white box over Pokemon for "faint" animation
			for (i = 0; i < 80; i++) {
				blinky_boi();
				check_pause();
				lcd_draw_rectangle(0, 100, 170, i, LCD_COLOR_WHITE); 
				for (j = 0; j < 100000; j++) {}
			}

			// Print out "Gengar has fainted"
			lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
			lcd_draw_string(gengarFaint, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);

			// Delay for readability
			for (j = 0; j < 1000000; j++){
				blinky_boi();
				check_pause();
			}

			// Erase Gengar's name
			lcd_draw_box(150,80,200,40, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0);

			// Initialize Ampharos
			lcd_draw_image(POKEMON_X_ALLY, ampharosWidthPixels,POKEMON_Y_ALLY,
			ampharosHeightPixels,ampharosBitmaps,LCD_COLOR_YELLOW, LCD_COLOR_WHITE);
			// Ampharos name
			lcd_draw_string(ampharos, 150, 210, LCD_COLOR_BLACK, LCD_COLOR_WHITE); 
		
			// Reset the Health Bar
			lcd_draw_rectangle(100, 120, 225, 15, LCD_COLOR_GREEN);
			ALLY_POKEMON_HEALTH = 120;

			return;


	// If Ampharos fainted
	case 'a':

		// Slowly draw white box over Pokemon for "faint" animation
		for (i = 0; i < 80; i++) {
			blinky_boi();
			check_pause();
			lcd_draw_rectangle(0, 100, 170, i, LCD_COLOR_WHITE); 
			for (j = 0; j < 100000; j++) {}
		}

		lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);

		// 'Ampharos has fainted'
		lcd_draw_string(amphFaint, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);

		// Erase Ampharos's name
		lcd_draw_box(140,80,200,40, LCD_COLOR_WHITE, LCD_COLOR_WHITE, 0);

		for (j = 0; j < 1000000; j++){
			blinky_boi();
			check_pause();
		}

		return;


	// If Charizard fainted
	case 'c':

		// Charizard faint 'animation'
		for (i = 0; i < 80; i++) {
			blinky_boi();
			check_pause();
			lcd_draw_rectangle(150, 85, 0, i, LCD_COLOR_WHITE); 
			for (j = 0; j < 100000; j++) {}
			}

		// Charizard has fainted
		lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
		lcd_draw_string(charFaint, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);

		for (j = 0; j < 1000000; j++){
			blinky_boi();
			check_pause();
		}

		// Erase Charizard name
		lcd_draw_box(0,80, 0, 40 , LCD_COLOR_WHITE, LCD_COLOR_WHITE,2);

		// Initialize Lapras
		lcd_draw_image(POKEMON_X_ENEMY, laprasWidthPixels, POKEMON_Y_ENEMY,
		laprasHeightPixels,laprasBitmaps,LCD_COLOR_WHITE, LCD_COLOR_BLUE);
		lcd_draw_string(lapras, 10, 30, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
		

		// Reset Health Bar
		lcd_draw_rectangle(10, 120, 50, 15, LCD_COLOR_GREEN);
		ENEMY_POKEMON_HEALTH = 120;

		return;

	// If Lapras fainted
	case 'l':

		// Lapras faint 'animation'
		for (i = 0; i < 100; i++) {
			blinky_boi();
			check_pause();
			lcd_draw_rectangle(150, 85, 0, i, LCD_COLOR_WHITE); 
			for (j = 0; j < 10000; j++) {}
		}

		// 'Lapras has fainted'
		lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
		lcd_draw_string(laprasFaint, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);

		// Erase Lapras's name
		lcd_draw_box(0,80, 0, 40 , LCD_COLOR_WHITE, LCD_COLOR_WHITE,2);

		for (j = 0; j < 1000000; j++){
			blinky_boi();
			check_pause();
		}

		return;

		default:
			break;

		return;
	}
}

// Main method for the game.  Simulates a single battle
// from the popular videogame series 'Pokemon"
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

	bool score_bool = true;

	int damageD = 0;  // Damage Done
	int damageT = 0;  // Damage Taken
	int ALLY_HEALTH_MAX = 120;


	// EEPROM
	eeprom_byte_read(I2C1_BASE, ADDR_START, &eeprom_data);
	printf("Trainer Gold Win's: %i\n", eeprom_data);
		
	// Initializes the game state
	battle_start();
	

	while(!game_over){
		
		enableLeds(pokemon_display);
		
		debounce_wait();
		
		if(d_pad->up == true){		
			d_pad->up = false;	
		}	

		if(d_pad->down == true){	
			d_pad->down = false;	
		}	

		if(d_pad->left == true){
			d_pad->left = false;	
		}	
		if(d_pad->right == true){		
			d_pad->right = false;	
		}	
		
		// Clean the text box
		lcd_draw_box(0,240,(ROWS-70), 70, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
		
		// Set up text box display for Gengar's moves
		if (allyPokemon == 'g') {
			lcd_draw_string(shadow_ball, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(hyper_beam, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(psychic, 145,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(protect, 145,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);	
    	}	

		// Set up text box display for Ampharos's moves
    	else {
			lcd_draw_string(thunderbolt, 25,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(zap, 25,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(dragon, 145,270, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
			lcd_draw_string(gem, 145,300, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
    } 

		n = rand() % 4; // Choose move for the enemy Pokemon
		switch(n) 
		{
			// All damage is under the assumption Pokemon health is 120 long
			case 0:

				effectMessage2 = '0';  // No special effect
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

					// If our Pokemon is Gengar
					else if(allyPokemon == 'g') {
						damageT = 75;
					}

					// If our Pokemon is Ampharos
					else {
						damageT = 95;
					}
				}

			case 1:

				if (enemyPokemon == 'c') {  // Earthquake for Charizard
					moveEnemy = '2';
					effectMessage2 = 's';  // Super effective

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
						damageT = 0;  // Missing does no damage
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
						effectMessage2 = 'u';  // It hurt itself with recoil
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
						effectMessage2 = '0';  
						damageT = 120;  // 1-hit KO
					}
				}	
			
				break;

			default:
				break;

		}
		
		move_select = true;
		// Await user input for the move
		while(move_select){		
			blinky_boi();
			check_pause();
			debounce_wait();
			cursor_draw();
			check_button(d_pad);

			// Top Left Move
			if(CURSE_X == 10 && CURSE_Y == 270) {
         	   position = 1;
        	}

			// Bottom Left Move
         	if (CURSE_X == 10 && CURSE_Y == 300) {
            	position = 2;
        	}

			// Top Right Move
         	if (CURSE_X == 130 && CURSE_Y == 270) {
            	position = 3;
        	}

			// Bottom Right Move
         	if (CURSE_X == 130 && CURSE_Y == 300) {
            	position = 4;
        	}

			if(d_pad->left){  // Select move
				move_select = false;
			}
		}
		
        // Assigns player's move selection to appropriate char
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
			
	
		// Assigns damage values based on player's move and enemy Pokemon
		switch(moveAlly) 
		{
			// All under the assumption Pokemon health is 120 long
			case 's': // Shadow Ball for Gengar

				effectMessage1 = '0'; // No special effect

				// Enemy Pokemon is Charizard
				if (enemyPokemon == 'c') {
					damageD = 65;
				}

				// Enemy Pokemon is Lapras
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

				// Can't use Protect 2 turns in a row
				if (lastMove == 'p') {
					effectMessage1 = 'm'; // 'But it missed'
				}

				// If Protect was succesful
				else {
					effectMessage1 = '0';
					effectMessage2 = 'p';  // Protect message 
					damageT = 0;  // Negates damage taken
				}
			
				break;


			case 't': // Thunderbolt for Ampharos

				effectMessage1 = 's';  // Super effective to both enemies
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
					effectMessage1 = 'm';  // 50% chance to miss
				}

				// Instantly kills Charizard if successful
				else if (enemyPokemon == 'c') {
					damageD = 120;
				}
				
				else {
					damageD = 80;
				}

				break;

			// Weakest Move
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

				effectMessage1 = 's'; // Super effective against both

				// x4 effective on Charizard 
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

		// Using Hyper Beam loses player their next turn
		if (lastMove == 'h') {
			damageD = 0;  // No damage this turn
			effectMessage1 = 'r';  // Recharge message
		} 

		// Keep track of last move
		lastMove = moveAlly;

		// Recoil Damage on Body Slam
		if (moveEnemy == '7' && effectMessage2 != 'p') {
			damageRecoil = 10;
		}

		// Ally Pokemon move execution
		printMoveMessage(allyPokemon, moveAlly, effectMessage1); // E.g.: Charizard used Flamethrower
		status = updateHealth(damageD, damageRecoil, 'E');  // Updates enemy Pokemon's health

		// Just in case player tried to use Hyper Beam twice
		if (effectMessage1 == 'r') {
			lastMove = '0';
		}

		// If enemy Pokemon fainted
		if (status == 'f') 		{
			status = 'n';  // Variable to cancel opponents next move on faint
			faintPokemon(enemyPokemon, enemyFaints);  
			enemyFaints += 1;
			pokemon_display &= 0x83;

			// Switch Pokemon to Lapras
			if (enemyFaints == 1) {
				enemyPokemon = 'l';

			}

			// Both Enemy Pokemon fainted
			else {
				// WE WIN
				pokemon_display &= 0x03;
				for(i = 0; i < 1000000; i++){}

				// Win Screen
				lcd_clear_screen(LCD_COLOR_BLACK);
				lcd_draw_string(win, 35,150, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
				lcd_draw_string(red_talk, 60,180, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
				printf("...\n");

				game_over = true;
				
			  	// Update Eeprom Score
				eeprom_data += 1;
				eeprom_byte_write(I2C1_BASE, ADDR_START, eeprom_data);
			}
		}

		// Ensures enemy Pokemon is not fainted
		if (status != 'n') {

			// Enemy's turn
			printMoveMessage(enemyPokemon, moveEnemy, effectMessage2);
			status = updateHealth(damageT, damageRecoil, 'A');  // Updates our Pokemon's health

			// If our Pokemon fainted
			if (status == 'f') {
				status = '0';  // Reset status
				faintPokemon(allyPokemon, allyFaints); 
				pokemon_display &= 0xC1; 
				allyFaints += 1;

				// Change to Ampharos
				if (allyFaints == 1) {
					allyPokemon = 'a';
				}

				// Both our Pokemon fainted - WE LOSE
				else {
					
					for(i = 0; i < 1000000; i++){}
					pokemon_display &= 0xC0;

					// Lose Screen
					lcd_clear_screen(LCD_COLOR_BLACK);
					lcd_draw_string(lose, 30,150, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
					game_over = true;	
				}
			}

			// Enemy Lapras fainted from Recoil on their turn
			else if (status == 'g') {
				status = '0';
				faintPokemon(enemyPokemon, enemyFaints);  // 'Enemy' Pokemon fainted 
				enemyFaints += 1;
				if (enemyFaints == 2) {
					// WE WIN
					pokemon_display &= 0x03;
					for(i = 0; i < 1000000; i++){}

					// Win screen
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

		// Reset damages
		damageD = 0;
		damageT = 0;
		damageRecoil = 0;
	}
	// End of game
	enableLeds(pokemon_display);
	while(1){
		blinky_boi();
	}
}