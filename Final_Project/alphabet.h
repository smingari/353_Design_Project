#ifndef __ALPHABET_H_
#define __ALPHABET_H_


#include <stdint.h>

// This structure describes a single character's display information
typedef struct
{
	const uint8_t widthBits;					// width, in bits of chars
	
	const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
	
} FONT_CHAR_INFO;	

extern const uint8_t alphabet_Bitmap[];
extern const FONT_CHAR_INFO alphabet_size[];

#endif 