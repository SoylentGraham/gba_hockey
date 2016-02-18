#ifndef CORE_TILES_H
#define CORE_TILES_H


#include "gba.h"





void scrollbackground( u16 x_scroll, u16 y_scroll, byte background );
void draw_tile(s16 x, s16 y, u8 tile_n, u8 background);

void clear_tile_map_row(byte tile_n, byte background, byte row);
void clear_tile_map(byte tile_n, byte background);
void draw_background( u8 tile_start, u8 background );




#endif

