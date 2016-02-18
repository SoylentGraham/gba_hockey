#include "gba.h"
#include "camera.h"
#include "sprites.h"
#include "map.h"

/*
//note: binding to a sprite, rather than a car, is probably better. 
i can make invisible fake sprites and move em about at my whim, rather than
forcing to make a new car entry.
 */

/*

//scroll data
s16 camera_tiny_x_scroll=0;
s16 camera_tiny_y_scroll=0;
s16 camera_x_scroll=0;
s16 camera_y_scroll=0;



//pointer to coords to follow (ie on a sprite)
s32 real_camera_save[2]={0};//for fixing and updating the camera's positon between frames
s32 *real_camera[2]={0};//needs shifting

s16 get_camera_x(void)
{
	s16 x,offset;
	offset = (VISIBLE_SCREEN_W/2);
	x= ((*real_camera[X])>> 8 );
	x-= offset;
	if ( x < 0 )
		return 0;
	if ( x+(offset*2) > (MAP_W*3)*TILE_W )
		return ((MAP_W*3)*TILE_W)-(offset*2);

	return  (x);
}

s16 get_camera_y(void)
{
	s16 y,offset;
	offset=(VISIBLE_SCREEN_H/2);
	y= ((*real_camera[Y])>> 8 );
	y-=offset;
	if ( y < 0 )
		return 0;
	if ( y+(offset*2) > (MAP_H*3)*TILE_H )
		return ((MAP_H*3)*TILE_H)-(offset*2);

	return  (y);
}


//updates reference of the camera to an associated sprite's coords
void unbind_camera(void)
{
	//bind the camera to the save. lets me modify camera pos still
	//(this should mess something up ;])
	real_camera[X] = &real_camera_save[X];
	real_camera[Y] = &real_camera_save[Y];
}

void bind_camera(a_car *car)
{
	//bind the camera coords to the sprite's coords
	real_camera[X]=&car->real_x;
	real_camera[Y]=&car->real_y;

	camera_x_scroll = CAMERA_TILE_X;
	camera_y_scroll = CAMERA_TILE_Y;
	camera_tiny_x_scroll = CAMERA_X - camera_x_scroll ;
	camera_tiny_y_scroll = CAMERA_Y - camera_y_scroll ;

	scrollbackground( (camera_x_scroll*8) + camera_tiny_x_scroll , (camera_y_scroll*8) + camera_tiny_y_scroll, ROAD_BACKGROUND );
	scrollbackground( (camera_x_scroll*8) + camera_tiny_x_scroll , (camera_y_scroll*8) + camera_tiny_y_scroll, ROOFTOP_BACKGROUND );
	scrollbackground( (camera_x_scroll*8) + camera_tiny_x_scroll , (camera_y_scroll*8) + camera_tiny_y_scroll, SHADOW_BACKGROUND );

	//if (old_camera[X] && old_camera[Y]) //used to be fixed onto a sprite
	{
		//reload the tilemap for the new position
		reset_game_tilemap( camera_x_scroll , camera_y_scroll );
	}
}


//save camera coords for scrolling update
void lock_camera(void)
{
	//not bound
	if (!real_camera[X] || !real_camera[Y]) 
		return;

	//	real_camera_save[X] = - (( *real_camera[X] )>>8)  ;
	//	real_camera_save[Y] = - (( *real_camera[Y] )>>8)  ;
	real_camera_save[X] = - CAMERA_X ;
	real_camera_save[Y] = - CAMERA_Y ;
}



//checks new camera coords against the coords when the camera was fixed, then updates scrolling and tiles
void release_camera(void) 
{
	s32 change[2];
	
	//no movement, cos the camera isnt bound
	if (!real_camera[X] || !real_camera[Y])		return; 

	//save is negative, adding produces number change around 0
	change[X] = real_camera_save[X] ;
	change[Y] = real_camera_save[Y] ;

	change[X] += CAMERA_X ;
	change[Y] += CAMERA_Y ;

	
//	change[X] = ( real_camera_save[X]>>8 ) + CAMERA_X ;	 
//	change[Y] = ( real_camera_save[Y]>>8 )  + CAMERA_Y ;
	//scroll, update background tilesets etc etc based on change[*]

	camera_tiny_x_scroll += change[X];
	camera_tiny_y_scroll += change[Y];

	
	while ( camera_tiny_x_scroll < 0 )
	{
		//gone left a tile
		camera_x_scroll--;
		camera_tiny_x_scroll = TINY_SCROLL_AMOUNT + camera_tiny_x_scroll;
		//refill tile next previous tile to the left
		reload_game_column( ROLL_SCREEN_NUMBER(camera_x_scroll-1) , ROLL_SCREEN_NUMBER(camera_y_scroll) , camera_x_scroll-1, camera_y_scroll);
	}

	while (camera_tiny_x_scroll>=TINY_SCROLL_AMOUNT)
	{
		//gone right a tile
		camera_x_scroll++;
		camera_tiny_x_scroll-=TINY_SCROLL_AMOUNT;
		//refill tile next tile to the right with the next
		reload_game_column( ROLL_SCREEN_NUMBER(camera_x_scroll+30) , ROLL_SCREEN_NUMBER(camera_y_scroll) , camera_x_scroll+30, camera_y_scroll );
	}


	//y
	while ( camera_tiny_y_scroll < 0 )
	{
		//gone up a tile
		camera_tiny_y_scroll = TINY_SCROLL_AMOUNT + camera_tiny_y_scroll;
		camera_y_scroll--;
		//refill tile next tile to the left for wrapping
		reload_game_row( ROLL_SCREEN_NUMBER(camera_x_scroll), ROLL_SCREEN_NUMBER(camera_y_scroll-1) , camera_x_scroll , camera_y_scroll-1);
	}


	while (camera_tiny_y_scroll>=TINY_SCROLL_AMOUNT)
	{
		//gone down a tile
		camera_tiny_y_scroll-=TINY_SCROLL_AMOUNT;
		camera_y_scroll++;
		//refill tile next tile to the right for wrapping
		reload_game_row( ROLL_SCREEN_NUMBER(camera_x_scroll) , ROLL_SCREEN_NUMBER(camera_y_scroll+30) , camera_x_scroll , camera_y_scroll+30);
	}


	scrollbackground( (camera_x_scroll*8) + camera_tiny_x_scroll , (camera_y_scroll*8) + camera_tiny_y_scroll, ROAD_BACKGROUND );
	scrollbackground( (camera_x_scroll*8) + camera_tiny_x_scroll , (camera_y_scroll*8) + camera_tiny_y_scroll, ROOFTOP_BACKGROUND );
	scrollbackground( (camera_x_scroll*8) + camera_tiny_x_scroll , (camera_y_scroll*8) + camera_tiny_y_scroll, SHADOW_BACKGROUND );



	//hide sprites that arent visible

	//or is that done per-sprite =/ ...
}

*/