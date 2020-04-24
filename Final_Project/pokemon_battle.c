#include "pokemon_battle.h"


volatile bool TIMER1_STATUS = true;

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
	
	
	
	while(!game_over){
		
		//lp_io_set_pin(RED_BIT);
		if(!TIMER1_STATUS){
		TIMER1_STATUS = true;
		lp_io_set_pin(BLUE_BIT);
	}
	
	else if(TIMER1_STATUS){
		TIMER1_STATUS = false;
		lp_io_clear_pin(BLUE_BIT);
	}
		
		
		

		lcd_draw_image(POKEMON_X_ALLY, charizardWidthPixels,POKEMON_Y_ALLY,
		charizardHeightPixels,charizardBitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
	}
	
	
}