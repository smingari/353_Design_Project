#include "ws2812b.h"

void ws2812b_rotate( 
  WS2812B_t *base,
  uint8_t num_leds
){
	WS2812B_t temp;
	int i;
	int j;
	for(i = 0; i < num_leds; i++){
		
		for(j = 0; j < num_leds - 1; j++){
			base[j] = base[j+1];
		}
		base[j] = temp;
	}

}

void ws2812b_pulse( 
  WS2812B_t *base,
  uint8_t num_leds
);
