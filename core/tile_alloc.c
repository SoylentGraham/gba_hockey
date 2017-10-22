#include "core.h"
#include "types.h"
#include "tile_alloc.h"

#include "../game/tile_sets.h"

//#include "../media/gfx/media.h"


const u8 max_tiles[2] = { 
	OBJ_MAX_TILES, BG_MAX_TILES, 
 };


u16 sys_brightness_mod=DEFAULT_BRIGHTNESS_MOD;




//custom addr's of media locations and stuff in game/tile_sets
extern const u8 tile_set_lengths[TILE_TYPE_COUNT];

extern u16 *tile_data_addr[2];
extern u16 *tile_set_data_addr[TILE_TYPE_COUNT];

extern u16 *tile_palette_addr[2];
extern u16 *tile_palette_data_addr[2];




//on-the-fly locations(offsets) of the tile_sets
u8 bg_tile_set[5][TILE_TYPE_COUNT] = {0};

//how many tiles are in use by the sets of tile data
u16 current_tile_data_count[5] = {0};



//load a set of tiles into either the background or the sprite char-data buffer
void load_tile_set(u8 mode,u8 tile_set)
{
	u16 i;
	//out of space for another [this] tileset
	//if ( current_tile_data_count[mode] + tile_set_lengths[tile_set] >= max_tiles[mode] ) return;

	//sets the "address" (offset) for this tile set on this mode
	bg_tile_set[mode][tile_set] = current_tile_data_count[mode];

	//load tiles into hardware data

	//using u16s instead of u8's, so *32 no *64
	for (i=0;i< 32*tile_set_lengths[tile_set] ;i++)
	{
		//if OBJ dont add charblock
		tile_data_addr[ mode ][ (current_tile_data_count[mode]*32)+i  ] = (tile_set_data_addr)[tile_set][i] ;
		//tile_data_addr[mode][ (current_tile_data_count[mode]*32)+i  ] = (tile_set_data_addr)[tile_set][i] ;
	}


	//increase current offset by the number of tiles we just added (aka: how many tiles we're using on this mode)
	current_tile_data_count[mode] += tile_set_lengths[tile_set];
}

//clears all. easier, and anyway, wont need to dispose of individual tile sets
void clear_current_tiledata(u8 mode)//mode is BG or SPRite
{
	//no need to overwrite anything
	current_tile_data_count[mode] = 0;
	//always load blank tiles
	//load_tile_set(mode,BLANK_TILES);
}



void load_tile_palette(s16 brightness_mod)
{
	//use one global palette, just to be easy
	// copy colour palettes
	u16 i;
	u16 orig_color, new_color;
	u16 new_rgb[3];
	s32 tmp;


	for (i=1; i<256; i++)
	{
		orig_color = tile_palette_data_addr[REAL_BG(0)][i];
/*		new_rgb[RED] = EXTRACT_RGB_16(orig_color,RED)+(brightness_mod-(MAX_BRIGHTNESS_MOD/2)) ;
		new_rgb[GREEN] = EXTRACT_RGB_16(orig_color,GREEN)+(brightness_mod-(MAX_BRIGHTNESS_MOD/2)) ;
		new_rgb[BLUE] = EXTRACT_RGB_16(orig_color,BLUE)+(brightness_mod-(MAX_BRIGHTNESS_MOD/2)) ;
*/		

		tmp = EXTRACT_RGB_16(orig_color,RED);
		if ( tmp + brightness_mod < 0 )		
			tmp = 0;
		else
			tmp += brightness_mod;
		new_rgb[RED] = (u16)tmp ;
		
		tmp = EXTRACT_RGB_16(orig_color,GREEN);
		if ( tmp + brightness_mod < 0 )		
			tmp = 0;
		else
			tmp += brightness_mod;
		new_rgb[GREEN] = (u16)tmp ;
		
		tmp = EXTRACT_RGB_16(orig_color,BLUE);
		if ( tmp + brightness_mod < 0 )		
			tmp = 0;
		else
			tmp += brightness_mod;
		new_rgb[BLUE] = (u16)tmp ;
		

/*		if ( new_rgb[RED] > MAX_BRIGHTNESS_MOD+MAX_RGB ) new_rgb[RED]=MIN_RGB;
		if ( new_rgb[GREEN] > MAX_BRIGHTNESS_MOD+MAX_RGB) new_rgb[GREEN]=MIN_RGB;
		if ( new_rgb[BLUE] > MAX_BRIGHTNESS_MOD+MAX_RGB ) new_rgb[BLUE]=MIN_RGB;

		if ( new_rgb[RED] > MAX_RGB ) new_rgb[RED]=MAX_RGB;
		if ( new_rgb[GREEN] > MAX_RGB ) new_rgb[GREEN]=MAX_RGB;
		if ( new_rgb[BLUE] > MAX_RGB ) new_rgb[BLUE]=MAX_RGB;
*/
		new_color = RGB16( new_rgb[RED], new_rgb[GREEN], new_rgb[BLUE] );

		tile_palette_addr[BG(0)][i] = new_color;
		tile_palette_addr[OBJ][i] = new_color;
		//tile_palette_addr[OBJ][i] = tile_palette_data_addr[OBJ][i];
	}


	//set pallete 0 to black (hides the green when loading etc)
	tile_palette_addr[BG(0)][0] = 0;
	tile_palette_addr[OBJ][0] = 0;
}

