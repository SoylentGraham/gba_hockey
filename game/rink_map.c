#include "rink_map.h"
#include "main.h"
#include "rink.h"
#include "game.h"
#include "crowd.h"
#include "tile_sets.h"
#include "hockey_sprites.h"
#include "../core/tile_alloc.h"
#include "../core/collisions.h"
#include "../core/camera.h"
#include "../core/keypad.h"
#include "../media/gfx/media.h"


const s16 tiny_scroll_offset_x[4] = { 
	0,	0,	0,	0,
};
//const
s16 tiny_scroll_offset_y[4] = { 
	0,	1,	0,	0,
};

//scroll data
s16 camera_tiny_x_scroll=0;
s16 camera_tiny_y_scroll=0;
s16 camera_x_scroll=0;
s16 camera_y_scroll=0;


//pointer to coords to follow (ie on a sprite)
s32 real_camera_save[2]={0};//for fixing and updating the camera's positon between frames
s32 current_camera_pos[2]; //for transition
s32 *real_camera[2]={0};//needs shifting

a_rink_map_data rink;


u8 coord_visible(s32 x, s32 y, s16 obj_w, s16 obj_h)
{
	if ( (x>>8) - CAM_SCROLL_PIXEL_X >= -obj_w )
		if ( ( (x>>8) - CAM_SCROLL_PIXEL_X) < (240) )
			if ( (y>>8) - CAM_SCROLL_PIXEL_Y >= -obj_h )
				if ( ( (y>>8) - CAM_SCROLL_PIXEL_Y) < (160) )
				{
					return 1;
				}
	return 0;
}




s16 rink_get_camera_x(void)
{
	s16 x,offset;
	offset = (VISIBLE_SCREEN_W/2);
	//x= ( *real_camera[X] )>>8;
	x= ( current_camera_pos[X] )>>8;
	x-= offset;


	if ( x < 0 )
		return 0;
//	if ( (x+(offset*2))>>8 > (RINK_W)*TILE_W )
//		return ((RINK_W)*TILE_W)-(offset*2)<<8;
	if ( (x+(offset*2)) > (RINK_W)*TILE_W )
		return ((RINK_W)*TILE_W)-(offset*2);


	return  (x);
}

s16 rink_get_camera_y(void)
{

	s16 y,offset;
	offset=(VISIBLE_SCREEN_H/2);
//	y= ( *real_camera[Y] )>>8;
	y= ( current_camera_pos[Y] )>>8;
	y-=offset;

	
	if ( y < 0 )
		return 0;
//	if ( (( y+(offset*2)) )>>8 > (RINK_H)*TILE_H )
//		return ((RINK_H)*TILE_H)-(offset*2)<<8;
	if ( (( y+(offset*2)) ) > (RINK_H)*TILE_H )
		return ((RINK_H)*TILE_H)-(offset*2);
		

	return  (y);

}


//updates reference of the camera to an associated sprite's coords
void rink_unbind_camera(void)
{
	//bind the camera to the save. lets me modify camera pos still
	//(this should mess something up ;])
	
	//real_camera[X] = &real_camera_save[X];
	//real_camera[Y] = &real_camera_save[Y];
	real_camera[X] = &current_camera_pos[X];
	real_camera[Y] = &current_camera_pos[Y];
}


void rink_bind_camera(s32 *bind_x, s32 *bind_y)
{

	//bind the camera coords to the sprite's coords
	real_camera[X] = bind_x;
	real_camera[Y] = bind_y;

}


//save camera coords for scrolling update
void rink_lock_camera(void)
{
	s32 change;
	//not bound
	if (!real_camera[X] || !real_camera[Y]) 
		return;


	real_camera_save[X] = - RINK_CAMERA_X ;
	real_camera_save[Y] = - RINK_CAMERA_Y ;

	//current_camera_pos[X] = *real_camera[X];
	//current_camera_pos[Y] = *real_camera[Y];
	current_camera_pos[X] += ( *real_camera[X] - current_camera_pos[X]) /20;
	current_camera_pos[Y] += ( *real_camera[Y] - current_camera_pos[Y]) /20;

	rink_release_camera();
}



//checks new camera coords against the coords when the camera was fixed, then updates scrolling and tiles
void rink_release_camera(void) 
{
	s32 change[2];
	u8 i;

	//no movement, cos the camera isnt bound
	if (!real_camera[X] || !real_camera[Y])		return; 

	

	//save is negative, adding produces number change around 0
	change[X] = real_camera_save[X] ;
	change[Y] = real_camera_save[Y] ;

	change[X] += RINK_CAMERA_X ;
	change[Y] += RINK_CAMERA_Y ;


	//scroll, update background tilesets etc etc based on change[*]

	camera_tiny_x_scroll += change[X];
	camera_tiny_y_scroll += change[Y];

	

	while ( camera_tiny_x_scroll < 0 )
	{
		//gone left a tile
		camera_x_scroll--;
		camera_tiny_x_scroll += TINY_SCROLL_AMOUNT ;
		//refill tile next previous tile to the left
		RELOAD_RINK_COL( ROLL_SCREEN_NUMBER(camera_x_scroll-1) , ROLL_SCREEN_NUMBER(camera_y_scroll) , camera_x_scroll-1, camera_y_scroll);
		
		if (DRAW_CROWD)
			update_crowd_column(0,false);
	}

	while (camera_tiny_x_scroll>=TINY_SCROLL_AMOUNT)
	{
		//gone right a tile
		camera_x_scroll++;
		camera_tiny_x_scroll -= TINY_SCROLL_AMOUNT;
		//refill tile next tile to the right with the next
		RELOAD_RINK_COL( ROLL_SCREEN_NUMBER(camera_x_scroll+30) , ROLL_SCREEN_NUMBER(camera_y_scroll) , camera_x_scroll+30, camera_y_scroll );

		if (DRAW_CROWD)
			update_crowd_column( VISIBLE_TILE_W,false );
	}


	//y
	while ( camera_tiny_y_scroll < 0 )
	{
		//gone up a tile
		camera_tiny_y_scroll = TINY_SCROLL_AMOUNT + camera_tiny_y_scroll;
		camera_y_scroll--;
		//refill tile next tile to the left for wrapping
		RELOAD_RINK_ROW( ROLL_SCREEN_NUMBER(camera_x_scroll), ROLL_SCREEN_NUMBER(camera_y_scroll-1) , camera_x_scroll , camera_y_scroll-1);

		if (DRAW_CROWD)
			for(i=0;i<CROWD_COLUMN_PER_ROW_UPDATE_RATE;i++)
				update_crowd_row(0,false,i);
	}


	while (camera_tiny_y_scroll>=TINY_SCROLL_AMOUNT)
	{
		//gone down a tile
		camera_tiny_y_scroll-=TINY_SCROLL_AMOUNT;
		camera_y_scroll++;
		//refill tile next tile to the right for wrapping
		RELOAD_RINK_ROW( ROLL_SCREEN_NUMBER(camera_x_scroll) , ROLL_SCREEN_NUMBER(camera_y_scroll+30) , camera_x_scroll , camera_y_scroll+30);
		
		//if (0)
		if (DRAW_CROWD)
			for(i=0;i<CROWD_COLUMN_PER_ROW_UPDATE_RATE;i++)
				update_crowd_row(VISIBLE_TILE_H,false,i);
	}

	
//	RELOAD_RINK( camera_x_scroll , camera_y_scroll );

	scrollbackground( (camera_x_scroll*8) + camera_tiny_x_scroll - tiny_scroll_offset_x[1], (camera_y_scroll*8) + camera_tiny_y_scroll - tiny_scroll_offset_y[1], 1 );
	scrollbackground( (camera_x_scroll*8) + camera_tiny_x_scroll - tiny_scroll_offset_x[2], (camera_y_scroll*8) + camera_tiny_y_scroll - tiny_scroll_offset_y[2], 2 );
	scrollbackground( (camera_x_scroll*8) + camera_tiny_x_scroll - tiny_scroll_offset_x[3], (camera_y_scroll*8) + camera_tiny_y_scroll - tiny_scroll_offset_y[3], 3 );



	//hide sprites that arent visible

	//or is that done per-sprite =/ ...
}




/*
#define COORD_IN_RINK(x,y) (coord_in_map(x,y,RINK_W,RINK_H))
u8 coord_in_map(s16 x, s16 y, u16 w, u16 h)
{
	if ( x<0 || y<0 || x>w || y>h )
		return false;
	return true;
}
*/






//BG_TILE( ICE_RINK_TILES ) + rink_data[ XY_ARRAY(x,y,RINK_W) ]
void rink_reload_bg_tiles_at( u16 sx, u16 sy, u16 mx, u16 my, u16 *data_map_a, u16 *data_map_b, u16 *data_map_c, u16 *data_map_d, u8 data_w, u8 data_h )
{

	if ( data_map_a )
		draw_tile( sx, sy, BG_TILE(0, rink.tileset[0] ) + data_map_a[ XY_ARRAY(mx, my, data_w) ]  , REAL_BG(0) );
	if ( data_map_b )
		draw_tile( sx, sy, BG_TILE(1, rink.tileset[1] ) + data_map_b[ XY_ARRAY(mx, my, data_w) ]  , REAL_BG(1) );
	if ( data_map_c )
		draw_tile( sx, sy, BG_TILE(2, rink.tileset[2] ) + data_map_c[ XY_ARRAY(mx, my, data_w) ]  , REAL_BG(2) );
	if ( data_map_d )
		draw_tile( sx, sy, BG_TILE(3, rink.tileset[3] ) + data_map_d[ XY_ARRAY(mx, my, data_w) ]  , REAL_BG(3) );

}




void rink_reload_column(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y,
//each corrasponds to which background its going onto
				u16 *data_map_a, u16 *data_map_b, u16 *data_map_c, u16 *data_map_d,
				u8 data_w, u8 data_h )
{		
	s16 y,x;
	u16 s_x,s_y;

	x=0;

	for ( y=0; y<FULL_TILE_H; y++)
	{
		s_x = ROLL_SCREEN_NUMBER( screen_x ) ;
		s_y = ROLL_SCREEN_NUMBER( screen_y + y ) ;

		rink_reload_bg_tiles_at( s_x, s_y, map_x+x, map_y+y, data_map_a, data_map_b, data_map_c, data_map_d, data_w, data_h );
	}	

	s_x = ROLL_SCREEN_NUMBER( screen_x ) ;
	s_y = ROLL_SCREEN_NUMBER( screen_y + 31 ) ;

	y=-1;
	rink_reload_bg_tiles_at( s_x, s_y, map_x+x, map_y+y, data_map_a, data_map_b, data_map_c, data_map_d, data_w, data_h );

}






void rink_reload_row(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y,
					 u16 *data_map_a, u16 *data_map_b, u16 *data_map_c, u16 *data_map_d,
					 u8 data_w, u8 data_h )
{	

	//map_x & map_x is the top of the column
	s16 x,y;
	u16 s_x,s_y;

	y=0;

	for ( x=0; x< FULL_TILE_W; x++)
	{
		s_x = ROLL_SCREEN_NUMBER( screen_x + x ) ;
		s_y = ROLL_SCREEN_NUMBER( screen_y ) ;

		rink_reload_bg_tiles_at( s_x, s_y, map_x+x, map_y+y, data_map_a, data_map_b, data_map_c, data_map_d, data_w, data_h );
	}

	s_x = ROLL_SCREEN_NUMBER(screen_x+31);
	s_y = ROLL_SCREEN_NUMBER(screen_y);

	x=-1;
	rink_reload_bg_tiles_at( s_x, s_y, map_x+x, map_y+y, data_map_a, data_map_b, data_map_c, data_map_d, data_w, data_h );

}





void rink_reset(s16 x_start, s16 y_start)
{
	s16 x, y;

	camera_tiny_x_scroll = camera_tiny_y_scroll = 0;
	camera_x_scroll = x_start;
	camera_y_scroll = y_start;

	for (x = 0; x < FULL_TILE_W; x ++)
	{
		RELOAD_RINK_COL(roll_s16(x+x_start,0,32),0,x+x_start,y_start);
	}

	RELOAD_RINK_COL( roll_s16(31+x_start,0,32),0,x_start-1, y_start);

}





void update_free_cam(void)
{
#define CAM_MOVE_RATE (5<<8)

	if (KEY_LEFT)
		current_camera_pos[X] -= CAM_MOVE_RATE;
	if (KEY_RIGHT)
		current_camera_pos[X] -= CAM_MOVE_RATE;
	
	if (KEY_UP)
		current_camera_pos[Y] -= CAM_MOVE_RATE;
	if (KEY_DOWN)
		current_camera_pos[Y] += CAM_MOVE_RATE;
}

