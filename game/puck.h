#ifndef GAME_PUCK_H
#define GAME_PUCK_H


#include "../core/gba.h"
#include "hockey_sprites.h"



typedef struct {
	u32 x; //large x
	u32 y;  //large y
	s32 x_speed;
	s32 y_speed;
	//u16 speed;
	//u16 angle; //0->360
//	u8 release; //ticks before puck can be picked up again, after launch etc
	//^^ moved to previous puck holder
	a_sprite *sprite; //sprite holder for this puck
	a_hockey_player *holder; //player currently holding the puck, or 0
} a_puck;



//#define PUCK_W 6
//#define PUCK_H 4
#define PUCK_W 8
#define PUCK_H 8

//mask offset
#define PUCK_X_OFFSET 0
#define PUCK_Y_OFFSET 0

#define PUCK_MIN_SPEED 20
#define PUCK_MAX_AFTER_BOUNCE 9999
#define PUCK_MAX_SPEED (6<<8) //never skip a tile, EVER

#define PLAYER_GOT_PUCK(player) ( PLAYER_IS_PLAYER(player,puck.holder) )

#define PUCK_SHOOT_SPEED 4
#define PUCK_PASS_SPEED 3
#define PUCK_LOSE_SPEED 1



u32 dist_from_puck( a_hockey_player *player );
//collisions
u8 check_tile_collisions_around_puck( u8 radius, u8 *result);
void reset_puck( u16 pixel_x, u16 pixel_y, a_sprite *allocated_sprite );
void update_puck(void);



#endif

