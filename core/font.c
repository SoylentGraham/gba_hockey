#include "gba.h"
#include "types.h"
#include "tiles.h"
#include <stdarg.h>

#include "font.h"
#include "tile_alloc.h"

extern u16 * tile_map[4];


u8 font_higher_limit[2];
u8 font_lower_limit[2];


unsigned char alphabet[ALPHABET_LENGTH]=
" abcdefghijklmnopqrstuvwxyz0123456789!?/.,\":$()-";


const u8 font_lookup[256] = {
  48,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   37,   42,    0,   44,    0,    0,    0,   45,   46,    0,    0,   41,   47,   40,   39,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   43,    0,    0,    0,    0,   38,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
};

static a_fontchar find_char_raw(char the_char)
{
	a_fontchar result;

/*	for (l=0; l<ALPHABET_LENGTH; l++) //go through eahc char 
		if ( alphabet[l] == the_char )
		{
			result.char_id=l;
			result.extra_data=0;
			return result;
		}
*/
	result.char_id = font_lookup[the_char];

	result.extra_data=0;
	return result;

}

byte get_font_tile(char letter, byte extra_data)
{
	a_fontchar font_char;

	if ( letter>='A' && letter<='Z' )
		letter+='a' - 'A';

	font_char.char_id = font_lookup[ letter ];

	if (font_char.char_id==0)
		return ( BG_TILE(HUD_BACKGROUND,BLANK_TILES) );

	return ( font_char.char_id + BG_TILE(HUD_BACKGROUND,FONT_TILES) -1 ); // -1 for space
}









void draw_text(u16 x, u16 y, u8 wrap, char *fmt, ...)
{
//	char newstring[30*28]; //max possible string
//	char newstring[30*1];
	char newstring[256];
	va_list	ap;

	if (fmt == NULL) return;

	va_start(ap, fmt);
	    vsprintf(newstring, fmt, ap);
	va_end(ap);

	draw_string(x, y, wrap, newstring);

	return;

	/*
	x_off=x;
	y_off=y;


	for (c=0; c<strlen(newstring); c++)
	{
		tile=get_font_tile( newstring[c], 0);

	//	if (coord_in_screen(x_off, y_off))
	//		if (tile_map[ HUD_BACKGROUND ][( (y_off) *(MAX_W))+ (x_off)] != tile) //only redraw if diff
				draw_tile( x_off, y_off, tile, HUD_BACKGROUND);

		x_off++;
		if (x_off >= 30) //visible across
		{
			if (!wrap) return;
			x_off=0;
			y_off++;
			//if (y_off>=28) return;
		}
	}
	*/

}



void draw_string(u16 x, u16 y, u8 wrap, char *string)
{
	u16 x_off,y_off;
	byte tile;
	u16 c;
	u8 len;


	len = strlen(string);

	//if ( font_lower_limit-font_higher_limit ==0 )		return;

	x_off=x;
	y_off=y;


	for (c=0; c<len; c++)
	{
		tile = get_font_tile( string[c], 0);

	//	if (coord_in_screen(x_off, y_off))
	//		if (tile_map[ HUD_BACKGROUND ][( (y_off) *(MAX_W))+ (x_off)] != tile) //only redraw if diff

		if ( tile_map[ HUD_BACKGROUND ][ XY_ARRAY(x_off,y_off,FULL_TILE_W) ] != tile )
			if ( y_off >= font_higher_limit[Y] && y_off <= font_lower_limit[Y] )
				draw_tile( x_off, y_off, tile, HUD_BACKGROUND);

		x_off++;
		//if (x_off >= VISIBLE_TILE_W) //visible across
		if (x_off >= font_lower_limit[X] ) //visible across
		{
			if (!wrap) return;
			x_off=0;
			y_off++;
			//if (y_off>=28) return;
		}
	}

}


void set_text_boundries( u8 y_high, u8 y_low, u8 x_high, u8 x_low )
{
	u8 line;
	font_higher_limit[X] = x_high;
	font_lower_limit[X] = x_low;
	font_higher_limit[Y] = y_high;
	font_lower_limit[Y] = y_low;
	//set some limit vars in the font funcs so they dont draw above the top, or below the bottom
	for (line = font_higher_limit[Y]; line<font_lower_limit[Y]; line++)
		clear_tile_map_row( BG_TILE(HUD_BACKGROUND,BLANK_TILES) , HUD_BACKGROUND, line);

}
