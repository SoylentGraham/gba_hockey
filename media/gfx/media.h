#ifndef MEDIA_MEDIA_H
#define MEDIA_MEDIA_H



#include "../../core/gba.h"
#include "../../core/types.h"


//palette
const u16 gfx_palette[256];

//generic tiles
const u16 font_tile_data[ 8*376 ];
const u16 font_scroll_1_tile_data[ 8*376 ];
const u16 font_scroll_2_tile_data[ 8*376 ];
const u16 font_scroll_3_tile_data[ 8*376 ];

const u16 blank_tile_data[ 8*16 ] ;
const u16 scoreboard_lights_tiles[8*248];


//sprite tiles
const u16 hockey_guy_body_data[ 8*640 ] ;
const u16 hockey_guy_head_data[ 8*200 ] ;
const u16 targets_tile_data[8*64];
const u16 arrows_tile_data[8*64];
const u16 puck_sprite_data[ 8*8 ] ;


//ice rink
const u16 ice_rink_screen_tile_data[ 8*208 ];
const u16 rink_overhead_tile_data[ 8*176 ];
const u16 ice_rink_tile_data[ 8*432 ];

//masks
const u16 generic_mask_data[ 8*8 ];
const u16 puck_mask_data[8*8] ;
const u16 ice_rink_mask_data[ 8*432 ];

//crowd
const u16 crowd_char1_head_tiles[ 8*256 ];
const u16 crowd_char1_body_tiles[ 8*256 ];
const u16 crowd_char2_head_tiles[ 8*256 ];
const u16 crowd_char2_body_tiles[ 8*256 ];
const u16 crowd_char3_head_tiles[ 8*256 ];
const u16 crowd_char3_body_tiles[ 8*256 ];


//hockey stick angles
const u16 hockey_sticks_tile_data[8*32];



//u8 masks
const u8 generic_mask_u8[8]; //8*y
const u8 rink_tile_mask_u8[432];
const u8 puck_mask_u8[8];

#define PUCK_MASK(n) ((u8*)puck_mask_data)





#endif
