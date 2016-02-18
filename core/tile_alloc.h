#ifndef TILE_ALLOC_H
#define TILE_ALLOC_H


#include "gba.h"
#include "../game/tile_sets.h"



extern u16 current_tile_data_count[5];


#define RED 0
#define GREEN 1
#define BLUE 2

#define EXTRACT_RGB_16(color,n) ((color>>(n*5)) & 31)
#define RGB16(r,g,b)  ((r)+(g<<5)+(b<<10))
//#define BGR(b,g,r) ((r)+((g)<<5)+((b)<<10)) //you can have 0-31 levels of each red,green,blue component.

#define MAX_BRIGHTNESS_MOD (u16)30
#define MIN_BRIGHTNESS_MOD (u16)0
#define MAX_RGB (u16)31
#define MIN_RGB (u16)0
#define DEFAULT_BRIGHTNESS_MOD (MAX_BRIGHTNESS_MOD/2)


//special tile numbers for set tiles
#define TRANSPARENT_TILE 0 //blank tile data
#define ALL_BLACK_TILE 1 //blank tile data and shadow data


extern u8 bg_tile_set[5][TILE_TYPE_COUNT];
#define BG_TILE(layer,set) (bg_tile_set[BG(layer)][set])

//#define OBJ_TILE(set,n) (((bg_tile_set[OBJ][set])*2)+n)
#define OBJ_TILE(set,n) (((bg_tile_set[OBJ][set])*1)+n)



void load_tile_set(u8 mode,u8 tile_set);
void clear_current_tiledata(u8 mode);
void load_tile_palette(s16 brightness_mod);




#endif

