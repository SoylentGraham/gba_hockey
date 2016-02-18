#include "types.h"
#include "tiles.h"
#include "tile_alloc.h"



u16 * tile_map[4]={
	SCREEN_BASE_BLOCK(0),
	SCREEN_BASE_BLOCK(1),
	SCREEN_BASE_BLOCK(2),
	SCREEN_BASE_BLOCK(3),
};




void scrollbackground( u16 x_scroll, u16 y_scroll, byte background )
{
	switch (background) {
	case 0: {
		REG_BG0HOFS=x_scroll;
		REG_BG0VOFS=y_scroll;
		return;
			}
	case 1: {
		REG_BG1HOFS=x_scroll;
		REG_BG1VOFS=y_scroll;
		return;
			}
	case 2: {
		REG_BG2HOFS=x_scroll;
		REG_BG2VOFS=y_scroll;
		return;
			}
	case 3: {
		REG_BG3HOFS=x_scroll;
		REG_BG3VOFS=y_scroll;
		return;
			}
	}
}



// draw a tile on the specified background
void draw_tile(s16 x, s16 y, u8 tile_n, u8 background)
{
	tile_map[background][XY_ARRAY(x,y,FULL_TILE_W)]=tile_n;
}




void clear_tile_map_row(byte tile_n, byte background, byte row)
{
	u16 x;

	for (x = 0; x < (FULL_TILE_W); x ++)
	{
		tile_map[background][XY_ARRAY(x,row,FULL_TILE_W)] = tile_n;
	}
}


void clear_tile_map(byte tile_n, byte background)
{
	u16 i;

	for (i = 0; i < (FULL_TILE_W*FULL_TILE_H); i ++)
	{
		tile_map[background][i] = tile_n;
	}
}






//fills background with a giant image (a tileset in the right size and order)
void draw_background( u8 tile_start, u8 background )
{
	u8 x,y;

	for ( y=0; y<20; y++)
		for ( x=0; x<30; x++)
		{
			//tile_map[background][ XY_ARRAY(x,y,30) ] = bg_tile_set[BG][ tile_set ] ; 
			tile_map[background][ XY_ARRAY(x,y,32) ] = tile_start + XY_ARRAY(x,y,30) ; 
		}
}

