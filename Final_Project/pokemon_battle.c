#include "pokemon_battle.h"

volatile bool TIMER1_ALERT =true;

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
	uint16_t TIMER1_COUNT = 0;
	int i = 0;
	
	
	
	while(!game_over){
		
		//lp_io_set_pin(RED_BIT);
	if(TIMER1_ALERT){
		
		TIMER1_ALERT = false;
		
		if (TIMER1_COUNT == 0){
			lp_io_set_pin(BLUE_BIT);
		}	
		else {
			lp_io_clear_pin(BLUE_BIT);
		}
		TIMER1_COUNT = (TIMER1_COUNT + 1) % 2;
		
	}
	
	// BOX 3/4 screen bottom left	
	lcd_draw_box(0,180,(ROWS-50), 50, LCD_COLOR_BLUE, LCD_COLOR_WHITE,2);
lcd_draw_image(POKEMON_X_ALLY, charizardWidthPixels,POKEMON_Y_ALLY,
charizardHeightPixels,charizardBitmaps,LCD_COLOR_BLACK,LCD_COLOR_RED);

for (i = 0; i < 1000; i++) {
}

lcd_draw_image(POKEMON_X_ALLY, laprasWidthPixels,POKEMON_Y_ALLY,
laprasHeightPixels,laprasBitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLUE);
	}
	
	
}