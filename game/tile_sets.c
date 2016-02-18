#include "tile_sets.h"
#include "../media/gfx/media.h"
//#include "../core/sprites.h"



extern u16 *oam_data;

#define TILE_DATA_SIZE( tile_obj ) ( sizeof(tile_obj) / (sizeof(u16)*64) )

const u8 tile_set_lengths[TILE_TYPE_COUNT] = {
	TILE_DATA_SIZE(	blank_tile_data ),
	TILE_DATA_SIZE(	hockey_guy_body_data ),
	TILE_DATA_SIZE(	hockey_guy_head_data ),
	TILE_DATA_SIZE(	font_tile_data ),
	TILE_DATA_SIZE(	puck_sprite_data ),
	TILE_DATA_SIZE(	targets_tile_data ),
	TILE_DATA_SIZE(	arrows_tile_data ),
	TILE_DATA_SIZE(	scoreboard_lights_tiles ),
	TILE_DATA_SIZE(	hockey_sticks_tile_data ),

	TILE_DATA_SIZE(	ice_rink_screen_tile_data ),
	TILE_DATA_SIZE(	rink_overhead_tile_data ),
	TILE_DATA_SIZE(	ice_rink_tile_data ),
	TILE_DATA_SIZE(	font_scroll_1_tile_data ),
	TILE_DATA_SIZE(	font_scroll_2_tile_data ),
	TILE_DATA_SIZE(	font_scroll_3_tile_data ),

	TILE_DATA_SIZE(	crowd_char1_head_tiles ),
	TILE_DATA_SIZE(	crowd_char1_body_tiles ),
	TILE_DATA_SIZE(	crowd_char2_head_tiles ),
	TILE_DATA_SIZE(	crowd_char2_body_tiles ),
	TILE_DATA_SIZE(	crowd_char3_head_tiles ),
	TILE_DATA_SIZE(	crowd_char3_body_tiles ),

};


const u16 *tile_set_data_addr[TILE_TYPE_COUNT] ={
	blank_tile_data,
	hockey_guy_body_data,
	hockey_guy_head_data,
	font_tile_data,
	puck_sprite_data,
	targets_tile_data,
	arrows_tile_data,
	scoreboard_lights_tiles,
	hockey_sticks_tile_data,

	ice_rink_screen_tile_data,
	rink_overhead_tile_data,
	ice_rink_tile_data,
	font_scroll_1_tile_data,
	font_scroll_2_tile_data,
	font_scroll_3_tile_data,

	crowd_char1_head_tiles,
	crowd_char1_body_tiles,
	crowd_char2_head_tiles,
	crowd_char2_body_tiles,
	crowd_char3_head_tiles,
	crowd_char3_body_tiles,

};



const u16 *tile_data_addr[1 + 4] = {
	((u16*)(0x6010000)), 
	CHAR_BASE_BLOCK(0),
	CHAR_BASE_BLOCK(1),
	CHAR_BASE_BLOCK(2),
	CHAR_BASE_BLOCK(1),
	//REG_TILE_BASE + (0x4000 * 1),
//	((u16*) (REG_TILE_BASE + 0x4000)), 
};


const u16 *tile_palette_data_addr[2] ={
	gfx_palette, gfx_palette,
};


const u16 *tile_palette_addr[2] = {
	((u16*)0x5000200), ((u16*)(REG_PALETTE_BASE)), 
};
