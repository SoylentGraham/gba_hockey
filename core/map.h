#ifndef CORE_MAP_H
#define CORE_MAP_H


#include "gba.h"
#include "../media/map/map_data.h"



//typedef a_road_tile u8;
//typedef a_shadow_tile u8;
//typedef a_building_tile u8;

typedef struct {
	u8
		road_tile_a: 4,
		road_tile_b: 4;
} a_road_tile;

typedef struct {
	u8 
		building_tile_n: 7, // tile num 0->128
		collision: 1;		//collidable, yes or no
} a_building_tile;

typedef struct {
	u8
		shadow_0: 3,
		shadow_1: 3,
		waste: 2;
} a_shadow_tile;





//works		24, 28, 32, 36, 
//complaint	26, 30, 34,
//blank		

#define MAP_W MEDIA_MAP_W
#define MAP_H MEDIA_MAP_H
//physical tile size is MAP_n * 3
//pixel size is MAP_n*3*8






void reload_column(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y, u8 *this_road_map, u8* this_building_map, u8* this_shadow_map, u8 road_bg, u8 building_bg, u8 shadow_bg, s16 map_w, s16 map_h);
void reload_row(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y, u8 *this_road_map, u8* this_building_map, u8* this_shadow_map, u8 road_bg, u8 building_bg, u8 shadow_bg, s16 map_w, s16 map_h );
void reset_tilemap(s16 x_start, s16 y_start, u8 *this_road_map, u8* this_building_map, u8* this_shadow_map, u8 road_bg, u8 building_bg, u8 shadow_bg, s16 map_w, s16 map_h );

void reload_game_row(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y);
void reload_game_column(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y);
void reset_game_tilemap(s16 x_start, s16 y_start);

u8 coord_in_map(s16 x, s16 y, u16 w, u16 h);


byte get_shadow_tile(const u8  *map, s16 x, s16 y,s16 map_w,s16 map_h);
byte get_building_tile(const u8 *map, s16 x, s16 y, s16 map_w,s16 map_h);
u8 get_building_tile_data(const u8 *map, s16 x, s16 y, s16 map_w,s16 map_h);
byte get_road_tile(const u8 *map, s16 x, s16 y,s16 map_w,s16 map_h);
byte get_road_data(const u8 *map, s16 x, s16 y,s16 map_w,s16 map_h);



#endif

