#ifndef GAME_RINK_H
#define GAME_RINK_H



#include "../core/gba.h"
#include "../core/types.h"
#include "../core/sprites.h"
#include "../core/animated_sprite.h"
#include "hockey_sprites.h"
#include "../media/map/map_data.h"


#define RINK_W MEDIA_MAP_W 
#define RINK_H MEDIA_MAP_H 

#define RINK_MASK(n) ((u8*) ((u8*)ice_rink_mask_data) + ( n * (sizeof(u8)*64) ) )

//use this to test every tile - worst [fps]case scenraio
//#define NOT_BLANK_RINK_TILE(t) ( 1 )
#define NOT_BLANK_RINK_TILE(t) ( t!=0 && t<37 )





void init_rink(void);
void redraw_rink(u8 background);
u8 check_tiles_around_hockey_player( a_hockey_player *player, u8 radius, u8 *result);





#endif

