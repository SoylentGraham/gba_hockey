#include "main.h"
#include "rink.h"
#include "rink_map.h"
#include "tile_sets.h"
#include "hockey_sprites.h"
#include "../core/tile_alloc.h"
#include "../core/collisions.h"
#include "../media/gfx/media.h"
#include "../core/math.h" //cos/sin
#include "../core/sprites.h"
#include "game.h"
#include "puck.h"
#include "tile_sets.h"



extern a_hockey_player team_players[TEAM_COUNT][PLAYER_COUNT/TEAM_COUNT];
extern const a_hockey_player *players;

a_puck puck; //the game puck!




void reset_puck( u16 pixel_x, u16 pixel_y, a_sprite *allocated_sprite ) //when reset, nobody has the puck
{
	//puck.angle = 0;
	puck.x_speed = 0;
	puck.y_speed = 0;
	puck.x = pixel_x <<8;
	puck.y = pixel_y <<8;
	puck.holder = 0;
	//puck.release = 0;
	puck.sprite = allocated_sprite;
	//init and show sprite
	init_sprite( puck.sprite, SIZE_8, 0, PUCK_TILES, 0, BODY_BG, 0, 0, 0 );
	move_sprite( puck.sprite, puck.x>>8, puck.y>>8 );
}


u32 dist_from_puck( a_hockey_player *player )
{
	return DISTANCE_FROM_COORD( (puck.x >>8), (puck.y >>8), (player->x >>8), (player->y >>8) );
}




//capture puck
u8 player_overlap_puck( a_hockey_player *player )
{
	u16 puck_coord[2]; //player/puck x/y
	u16 player_match_coord[2]; //stick connection
	u8 holder_stick_ang_offset;

	puck_coord[X] = (puck.x>>8) + PUCK_SPRITE_CENTER;
	puck_coord[Y] = (puck.y>>8) + PUCK_SPRITE_CENTER;

	holder_stick_ang_offset = roll_u8 ( player->stick_angle - (player->stick_tick/STICK_SWING_RATE) , 0 ,8);
	player_match_coord[X] = (player->x>>8) + (PUCK_POS_ON_STICK_X( holder_stick_ang_offset , player->stick_in));
	player_match_coord[Y] = (player->y>>8) + (PUCK_POS_ON_STICK_Y( holder_stick_ang_offset , player->stick_in));


	//if no chance of overlap, return
	if ( player_match_coord[X] < puck_coord[X] - PUCK_STICK_TOLERANCE ) return false;
	if ( player_match_coord[X] > puck_coord[X] + PUCK_STICK_TOLERANCE ) return false;

	if ( player_match_coord[Y] < puck_coord[Y] - PUCK_STICK_TOLERANCE ) return false;
	if ( player_match_coord[Y] > puck_coord[Y] + PUCK_STICK_TOLERANCE ) return false;


	return true;
}


u8 check_puck_player_collisions(void) 
//check all players to see if theyre overlapping the puck
{
	u8 p,pp;

	if (!GAME_IN_PLAY)
		return false;

	for (p=0; p<PLAYER_COUNT; p++)
	{
		//draw_text(0,10,true,"%d",p);

		if ( player_overlap_puck( (a_hockey_player*)&players[p] ) && players[p].release_timer==0 )
		{
			puck.holder = (a_hockey_player*)&players[p];

			if ( AUTO_SWITCH_CONTROL )
				if ( puck.holder->team_no == CONTROLLED_PLAYER_TEAM )
				{
					//update control to puck.holder
					switch_local_controlled_player( puck.holder, true );
				}

			return true;
		}
		
	}

	return false;
}



void update_puck(void)
{
	u8 tile_collision_side;
	u8 goal_check = false;
	u8 hit_goal;
	u8 holder_stick_ang_offset;

	if (!puck.holder)
	{
		//find a new owner if any
		if ( check_puck_player_collisions() )
			return;
		/*
		if ( puck.holder->release_timer == 0)
		{
			if ( check_puck_player_collisions() )
				return;
		} else {
			puck.release--;
		}
		*/
		
		//check angle
		//puck.angle = roll_u16(puck.angle,0,360);

		if ( check_tile_collisions_around_puck( 1, &tile_collision_side) )
		{
			goal_check = true;

			if ( tile_collision_side & DIR_BIT_GOAL )
			{
				//GOOOAAALLLLL!!!!
				if ( tile_collision_side == DIR_BIT_GOAL )
				{
					//work out which goal
					if ( (puck.y>>8) < (RINK_H*TILE_H)/2 ) 
						hit_goal = 1;
					else
						hit_goal = 2;
				} else {
					hit_goal = tile_collision_side - DIR_BIT_GOAL ;
				}

				score_goal( hit_goal );
			}

			modify_xy_speed_on_collision( tile_collision_side , &puck.x_speed, &puck.y_speed, -10, PUCK_MIN_SPEED, PUCK_MAX_AFTER_BOUNCE );
		}

		//travel along the rink
		//puck.x += ( COS[puck.angle] * puck.speed ) >> 8;
		//puck.y += ( SIN[puck.angle] * puck.speed ) >> 8;
		if ( puck.x_speed < PUCK_MAX_SPEED)
			puck.x += puck.x_speed;
		else
			puck.x += PUCK_MAX_SPEED;

		if ( puck.y_speed < PUCK_MAX_SPEED)
			puck.y += puck.y_speed;
		else
			puck.y += PUCK_MAX_SPEED;


		//slowdown
		if ( puck.x_speed > PUCK_MIN_SPEED )
			puck.x_speed -= 1;
		if ( puck.x_speed < -PUCK_MIN_SPEED )
			puck.x_speed += 1;

		if ( puck.y_speed > PUCK_MIN_SPEED )
			puck.y_speed -= 1;
		if ( puck.y_speed < -PUCK_MIN_SPEED )
			puck.y_speed += 1;

		//move_sprite( puck.sprite, puck.x>>8, puck.y>>8 );

	} else {

		holder_stick_ang_offset = roll_u8 ( puck.holder->stick_angle - (puck.holder->stick_tick/STICK_SWING_RATE) , 0 ,8);
		//player moves for us
		puck.x = puck.holder -> x + ((PUCK_POS_ON_STICK_X( holder_stick_ang_offset , puck.holder->stick_in))<<8);
		puck.y = puck.holder -> y + ((PUCK_POS_ON_STICK_Y( holder_stick_ang_offset , puck.holder->stick_in))<<8);

		
		//hide_sprite( puck.sprite );
	}


	if ( !goal_check )
	{
		if ( check_tile_collisions_around_puck( 1, &tile_collision_side) )
		{
			if ( tile_collision_side & DIR_BIT_GOAL )
			{
				if ( tile_collision_side == DIR_BIT_GOAL )
				{
					//work out which goal
					if ( (puck.y>>8) < (RINK_H*TILE_H)/2 ) 
						hit_goal = 1;
					else
						hit_goal = 2;

				} else {
					hit_goal = tile_collision_side - DIR_BIT_GOAL ;
				}

				score_goal( hit_goal );
			}
		}
	}
 

	//is backwards =/
	if (! coord_visible(puck.x, puck.y, PUCK_W, PUCK_H) )
		hide_sprite( puck.sprite );
	else
		move_sprite( puck.sprite, (puck.x>>8)-(RINK_CAMERA_X) , (puck.y>>8)-(RINK_CAMERA_Y) );


}


//collisions
u8 check_tile_collisions_around_puck( u8 radius, u8 *result)
{
	s16 x,y;//loop
	s16 tile_x,tile_y;
	u16 tile;
	u8 crash_id[2];
	s16 p[2][2];
	u8 i;
	
	p[0][X] = (puck.x>>8) - 4 + PUCK_X_OFFSET ; //left of head
	p[0][Y] = (puck.y>>8) - 4 + PUCK_Y_OFFSET ;

	tile_x = p[0][X] / 8; //nearest tile
	tile_y = p[0][Y] / 8;

	tile_x++;
	tile_y++;

	i=0;

	if ( tile_x < 0 )	
	{
		*result |= DIR_BIT_W;
		i=true;
	}
	if ( tile_x >= RINK_W )	
	{
		*result |= DIR_BIT_E;
		i=true;
	}

	if ( tile_y < 0 )	
	{
		*result |= DIR_BIT_N;
		i=true;
	}
	if ( tile_y >= RINK_H )	
	{
		*result |= DIR_BIT_S;
		i=true;
	}

	
	if ( i )	return true;



	for ( x = tile_x-radius ; x<= tile_x+radius; x++)
	{
		for ( y = tile_y-radius ; y<= tile_y+radius; y++)
		{
			if ( x>=0 && y>=0 && x<RINK_W && y<RINK_H )
			{
				tile = rink.mask[ XY_ARRAY(x,y,RINK_W) ];
				if ( NOT_BLANK_RINK_TILE(tile) )
				{
					p[1][X] = ( x*TILE_W ) + 0;
					p[1][Y] = ( y*TILE_H ) + 0;
 
					
					
					//if ( check_collisions ( PUCK_W, TILE_W, PUCK_H, TILE_H,	PUCK_MASK(0), RINK_MASK(tile), p[0][X] , p[0][Y], p[1][X], p[1][Y], &crash_id[0], result, generic_mask_u8, rink_tile_mask_u8 ) )
					if ( check_collisions ( PUCK_W, TILE_W, PUCK_H, TILE_H,	PUCK_MASK(0), RINK_MASK(tile), p[0][X] , p[0][Y], p[1][X], p[1][Y], &crash_id[0], result, puck_mask_u8, rink_tile_mask_u8 ) )
					//if ( check_collisions ( PUCK_W, TILE_W, PUCK_H, TILE_H,	PUCK_MASK(0), RINK_MASK(tile), p[0][X] , p[0][Y], p[1][X], p[1][Y], &crash_id[0], result, 0, 0 ) )
						return true;
				}
			}
		}
	}
			

	return false;
}

