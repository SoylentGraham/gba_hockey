#ifndef GAME_TILE_SETS_H
#define GAME_TILE_SETS_H


#include "../core/gba.h"




enum {
	BLANK_TILES=0,
	HOCKEY_GUY_BODY_TILES,
	HOCKEY_GUY_HEAD_TILES,
	FONT_TILES,
	PUCK_TILES,
	TARGET_TILES,
	ARROW_TILES,
	SCOREBOARD_TILES,
	HOCKEY_STICK_TILES,

	ICE_RINK_SCREEN_TILES,
	ICE_RINK_OVERHEAD_TILES,
	ICE_RINK_FLOOR_TILES,

	FONT_SCROLL_1_TILES,
	FONT_SCROLL_2_TILES,
	FONT_SCROLL_3_TILES,

	CROWD_CHAR1_HEAD_TILES,
	CROWD_CHAR1_BODY_TILES,
	CROWD_CHAR2_HEAD_TILES,
	CROWD_CHAR2_BODY_TILES,
	CROWD_CHAR3_HEAD_TILES,
	CROWD_CHAR3_BODY_TILES,

	TILE_TYPE_COUNT,
};

extern u16 *tile_set_data_addr[TILE_TYPE_COUNT];



#define CAR_TILES 0
#define ROAD_TILES 0
#define BUILDING_TILES 0
#define SHADOW_TILES 0
#define SPOTLIGHT_TILES 0




#endif

