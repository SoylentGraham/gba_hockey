#include "main.h"
#include "rink.h"
#include "rink_map.h"
#include "game.h"
#include "tile_sets.h"
#include "hockey_sprites.h"
#include "scoreboard.h"

#include "../core/tile_alloc.h"
#include "../core/collisions.h"
#include "../core/dma.h"

#include "../media/gfx/media.h"




extern a_hockey_player team_players[TEAM_COUNT][PLAYER_COUNT/TEAM_COUNT];
extern const a_hockey_player *players;


#define BLANK_ICE_RINK_TILE ( DIR_COUNT + DIR_COUNT + 1 + 18 )



extern const u16 rink_markings[ ICE_RINK_W * ICE_RINK_H ];




u16 goal_data[2][ GOAL_W*GOAL_H ] = {
	0,	3,	3,	3,	3,	6, 
	1,	4,	4,	4,	4,	7,
	2,	5,	5,	5,	5,	8,

	9,	12,	12,	12,	12,	15, 
	10,	13,	13,	13,	13,	16,
	11,	14,	14,	14,	14,	17,
};


const u16 blank_floor[ MEDIA_MAP_W * MEDIA_MAP_H ] = { 1 };

u16 floor_tile_buffer[ MEDIA_MAP_W * MEDIA_MAP_H ];
u16 overhead_tile_buffer[ MEDIA_MAP_W * MEDIA_MAP_H ];


void init_rink(void)
{
	//load data
	u8 i,offset,layer;
	u16 a,x,y,aa;

	rink.mask = &def_rink_floor;
	rink.screen_tiles = &def_rink_screen;
	rink.above_tiles = &def_rink_overhead;
	rink.floor_tiles = &def_rink_floor;

	DMACopyU16( rink.floor_tiles, &floor_tile_buffer, MEDIA_MAP_W, MEDIA_MAP_H );
	DMACopyU16( rink.above_tiles, &overhead_tile_buffer, MEDIA_MAP_W, MEDIA_MAP_H );
	//memcpy( &floor_tile_buffer, rink.floor_tiles, sizeof(u16) * (MEDIA_MAP_W * MEDIA_MAP_H) );
	//memcpy( &overhead_tile_buffer, rink.above_tiles, sizeof(u16) * (MEDIA_MAP_W * MEDIA_MAP_H) );

	rink.floor_tiles = &floor_tile_buffer;
	rink.above_tiles = &overhead_tile_buffer;

	
	rink.tileset[0] = ICE_RINK_FLOOR_TILES;
	rink.tileset[1] = ICE_RINK_SCREEN_TILES;
	rink.tileset[2] = ICE_RINK_OVERHEAD_TILES;
	rink.tileset[3] = ICE_RINK_FLOOR_TILES;
/*
	for (x=0;x<RINK_W;x++)
	{
		for (y=0;y<RINK_H;y++)
		{
			rink.above_tiles[ XY_ARRAY(x,y,RINK_W) ] = BLANK_ICE_RINK_TILE ;
		}
	}

	for (offset=0; offset< RINK_EDGE_WIDTH; offset++)
	{
		draw_offset:
		for ( i=offset; i<RINK_W-offset; i++)
		{
			rink.mask[ XY_ARRAY(i,offset,RINK_W) ] = TEMP( NORTH+DIR_COUNT  );
			rink.mask[ XY_ARRAY(i,RINK_H-1-offset,RINK_W) ] = TEMP( SOUTH+DIR_COUNT );
		}
		//top/bottom row
		for ( i=offset; i<RINK_H-offset; i++)
		{
			rink.mask[ XY_ARRAY(offset,i,RINK_W) ] = TEMP( WEST+DIR_COUNT );
			rink.mask[ XY_ARRAY(RINK_W-1-offset,i,RINK_W) ] = TEMP( EAST+DIR_COUNT );
		}
		//corners
		rink.mask[ XY_ARRAY( offset, offset, RINK_W) ] = TEMP( NORTH_WEST+DIR_COUNT  );
		rink.mask[ XY_ARRAY( RINK_W-1-offset, offset, RINK_W) ] = TEMP( NORTH_EAST+DIR_COUNT  );
		rink.mask[ XY_ARRAY( RINK_W-1-offset, RINK_H-1-offset, RINK_W) ] = TEMP( SOUTH_EAST+DIR_COUNT  );
		rink.mask[ XY_ARRAY( offset, RINK_H-1-offset, RINK_W) ] = TEMP( SOUTH_WEST+DIR_COUNT  );

//		if (offset==13)	goto skip_draw;
	}


//	offset=13;
//	goto draw_offset;
//	skip_draw:



	for ( x=0; x<GOAL_W; x++)
	{
		for ( y=0; y<GOAL_H; y++)
		{
			rink.mask[ XY_ARRAY( x+GOAL_L,y+GOAL_T_A,RINK_W ) ] = goal_data[0][ XY_ARRAY( x,y,GOAL_W ) ] + DIR_COUNT + DIR_COUNT + 1;
			rink.mask[ XY_ARRAY( x+GOAL_L,y+GOAL_T_B,RINK_W ) ] = goal_data[1][ XY_ARRAY( x,y,GOAL_W ) ] + DIR_COUNT + DIR_COUNT + 1;

			rink.above_tiles[ XY_ARRAY( x+GOAL_L,y+GOAL_T_A,RINK_W ) ] = goal_data[0][ XY_ARRAY( x,y,GOAL_W ) ] + DIR_COUNT + DIR_COUNT + 1 ;

			if ( y!=GOAL_H-1 )
				rink.above_tiles[ XY_ARRAY( x+GOAL_L,y+GOAL_T_B,RINK_W ) ] = goal_data[1][ XY_ARRAY( x,y,GOAL_W ) ] + DIR_COUNT + DIR_COUNT + 1 ;
		}
	}






	//rink bars	
	for ( i=offset; i<RINK_W-offset; i++)
	{
		rink.mask[ XY_ARRAY(i,offset,RINK_W) ] = TEMP( NORTH );
		rink.mask[ XY_ARRAY(i,RINK_H-1-offset,RINK_W) ] = TEMP( SOUTH );
	}
	//top/bottom row
	for ( i=offset; i<RINK_H-offset; i++)
	{
		rink.mask[ XY_ARRAY(offset,i,RINK_W) ] = TEMP( WEST );
		rink.mask[ XY_ARRAY(RINK_W-1-offset,i,RINK_W) ] = TEMP( EAST );
	}
	//corners
	rink.mask[ XY_ARRAY( offset, offset, RINK_W) ] = TEMP( NORTH_WEST  );
	rink.mask[ XY_ARRAY( RINK_W-1-offset, offset, RINK_W) ] = TEMP( NORTH_EAST  );
	rink.mask[ XY_ARRAY( RINK_W-1-offset, RINK_H-1-offset, RINK_W) ] = TEMP( SOUTH_EAST  );
	rink.mask[ XY_ARRAY( offset, RINK_H-1-offset, RINK_W) ] = TEMP( SOUTH_WEST  );






	for (x=0; x<RINK_W; x++)
	{
		for(y=0; y<RINK_H; y++)
		{
			a = XY_ARRAY( x,y,RINK_W );
/*
			//includes those borders, so dont set rink_mask to 0
			if ( x>RINK_EDGE_WIDTH && x<RINK_W-RINK_EDGE_WIDTH-1
				&& 	y>RINK_EDGE_WIDTH && y<RINK_H-RINK_EDGE_WIDTH-1
				)
			{
				//ice
				//rink.mask[a] = 0; //no collision
				rink.ice_tiles[a] = 0; //plain ice
				aa = XY_ARRAY( x-RINK_EDGE_WIDTH,y-RINK_EDGE_WIDTH,ICE_RINK_W );
				rink.floor_tiles[a] = rink_markings[aa];

			} else {
				rink.floor_tiles[a] = rink.mask[a];
				rink.ice_tiles[a] = rink.mask[a];
			}
//* /
				rink.floor_tiles[a] = rink.mask[a];
				rink.ice_tiles[a] = rink.mask[a];
				if ( rink.floor_tiles[ XY_ARRAY( x,y+1,RINK_W ) ] == 5 )
					rink.screen_tiles[a] = 1; 
				else
					rink.screen_tiles[a] = 0;

		}
	}


	for ( x=0; x<GOAL_W; x++)
		for ( y=0; y<GOAL_H; y++)
		{
			a = XY_ARRAY( x+GOAL_L,y+GOAL_T_A,RINK_W );
			rink.floor_tiles[a] = 0;
			rink.ice_tiles[a] = 0;

			if ( y!=GOAL_H-1 )
			{
				a = XY_ARRAY( x+GOAL_L,y+GOAL_T_B,RINK_W );
				rink.floor_tiles[a] = 0;
				rink.ice_tiles[a] = 0;
			}
		}


*/
}







u8 check_and_handle_rink_collisions( a_hockey_player *player )
{
	u8 result=0;

	//if (0)
	if ( check_tiles_around_hockey_player( player, 1, &result) )
	{
		modify_xy_speed_on_collision( result, &player->x_speed, &player->y_speed, 40, PLAYER_MIN_SPEED , PLAYER_MAX_BOUNCE_SPEED );
	}

	return result;
}





u8 check_tiles_around_hockey_player( a_hockey_player *player, u8 radius, u8 *result)
{
	s16 x,y;//loop
	s16 tile_x,tile_y;
	u8 crash_id[2];
	s16 p[2][2];
	u8 i;
	u16 rink_val;


	
	p[0][X] = (player->x>>8) - 4 + PLAYER_X_OFFSET ; //left of head
	p[0][Y] = (player->y>>8) - 4 + PLAYER_Y_OFFSET ; //top of head


	//nearest tile to this players pixel
	tile_x = p[0][X] / TILE_W; 
	tile_y = p[0][Y] / TILE_H;

	tile_x ++;
	tile_y ++;

	x = tile_x - radius;
	if (x<0) x=0;

	//change to start from center and expand
	while( x<tile_x+radius )
	{
		p[1][X] = ( x*TILE_W ) + 0; 

		y = tile_y-radius;
		if (y<0) y=0;

		while( y<tile_y+radius)
		{
			//mask value of this tile
			rink_val = rink.mask[ XY_ARRAY(x,y,RINK_W) ];
 			
			if ( NOT_BLANK_RINK_TILE(rink_val) ) //mask isnt blank, saves checking empty ones
			{
				//tile top left
				p[1][Y] = ( y*TILE_H ) + 0;

				//if ( check_collisions ( PLAYER_MASK_W, TILE_W, PLAYER_MASK_H, TILE_H,	GENERIC_MASK, RINK_MASK(rink_val), p[0][X] , p[0][Y], p[1][X], p[1][Y], &crash_id[0], result , ((u8*)generic_mask_u8) , ((u8*)(rink_tile_mask_u8[rink_val*8])) ) )
				if ( check_collisions ( PLAYER_MASK_W, TILE_W, PLAYER_MASK_H, TILE_H,	GENERIC_MASK, RINK_MASK(rink_val), p[0][X] , p[0][Y], p[1][X], p[1][Y], &crash_id[0], result , generic_mask_u8 , rink_tile_mask_u8 ) )
					return true;

			}


			y++;
			if (y>=RINK_H)
				break;
		}


		x++;
		if (x>=RINK_W)
			break;
	}
			

	return false;
}




