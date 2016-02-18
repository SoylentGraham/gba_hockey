
#include "types.h"
#include "car.h"
#include "keypad.h"
#include "collisions.h"
#include "math.h"
#include "camera.h"
#include "../game/tile_sets.h"

extern s16 camera_tiny_x_scroll;
extern s16 camera_tiny_y_scroll;
extern s16 camera_x_scroll;
extern s16 camera_y_scroll;

extern u8 building_map[];

a_car *player_controlled_car=0;




/*

void set_car_type( a_car_type *car_type_data, u8 sprite_tile_start, s32 accel_rate, s32 turn_rate )
{
	car_type_data->accel_rate=accel_rate;
	car_type_data->sprite_tile_start=sprite_tile_start;
	car_type_data->turn_rate=turn_rate;
}


void bind_player_car(a_car *players_car)
{
	player_controlled_car=players_car;
}


void update_players_car(void)
{
	if (!player_controlled_car) return; //car isnt bound to a player

	player_controlled_car->movement = MOVE_NONE;

	if ( KEY_UP )
		player_controlled_car->movement |= ACCEL_FOWARD;
	
	if ( KEY_DOWN )
		player_controlled_car->movement |= ACCEL_BACKWARD;

	if ( KEY_LEFT )
		player_controlled_car->movement |= STEER_LEFT;
	
	if ( KEY_RIGHT )
		player_controlled_car->movement |= STEER_RIGHT;

	if ( KEY_B )
		player_controlled_car->movement |= HANDBRAKE;


}

//returns my new car!
void init_car( a_car *car, a_sprite *sprite, a_car_type *car_type , s16 init_tile_x, s16 init_tile_y, u16 init_ang, u8 collide_data, u8 ai_mode)
{
	if (!car || !sprite || !car_type) return;

	car->car_type = car_type;
	car->sprite = sprite;

	car->ai_mode=ai_mode;
	
	car->driving_ang = init_ang<<8;
	car->travel_ang = init_ang<<8;
	
	car->real_speed = 0;
	car->real_x = (init_tile_x*TILE_W)<<8;
	car->real_y = (init_tile_y*TILE_H)<<8;

	car->steering_ang = 0;
	
	car->movement = MOVE_NONE;
	
	car->collision_data = collide_data;
	get_sprite_rot_id(sprite, &car->rot_id);
	
	init_sprite( car->sprite, SIZE_16, SQUARE, CAR_TILES, 0, 3, car->rot_id, false, false);
	rotate_sprite( sprite, init_ang, ZOOM(1) ,ZOOM(1) );

}



#define SLOWDOWN 20
#define TURN_SLOW 100

void process_car(a_car *car)
{
	u8 coll_id[2]; //collision id's
	s16 coll_tile[2]; //x/y
	
	//moving foward
	if ( car->movement & ACCEL_FOWARD ) {
		car->real_speed += car->car_type->accel_rate ;
   	}
	if ( car->movement & ACCEL_BACKWARD ) {
		car->real_speed -= car->car_type->accel_rate / 2 ;
   	}

	//rotate
	if ( car->movement & STEER_LEFT ) {
		car->driving_ang -= 4<<8 ;
	
		if ( abs( car->real_speed ) < car->car_type->turn_rate ) 
			car->real_speed -= 1 ;
		else
			car->real_speed -= car->real_speed / car->car_type->turn_rate ;

	}

	if ( car->movement & STEER_RIGHT ) {
			
		car->driving_ang += 4<<8 ;

		if ( abs(car->real_speed) < car->car_type->turn_rate ) 
			car->real_speed -= 1 ;
		else
			car->real_speed -= car->real_speed / car->car_type->turn_rate;

	}


	//re_align_steering( sprite ); //not working yet
	
	car->travel_ang = car->driving_ang;

	if (car->collision_data & COLLIDE_BUILDING )
		if ( check_tiles_around_car( &coll_id[0], &coll_id[1],  &coll_tile[X], &coll_tile[Y] , car, 2) )
		{
			//collided with a tile at id coll_id[0]
			if ( coll_id[0] & COL_FRONT  || coll_id[0] & COL_BACK || coll_id[0] & COL_LEFT  || coll_id[0] & COL_RIGHT )
			{
				//most quickest hackiest collision reaction in the west
				car->real_speed = -car->real_speed;
			}

			//if ( coll_id[0] & COL_LEFT  || coll_id[0] & COL_RIGHT )
			//	sprite->travel_ang += 45 << 8;
		}


	car->travel_ang = roll_s32( car->travel_ang ,0, 360<<8 );


	//slow down, move etc
	if ( car->real_speed != 0 )
	{
		//rotate here

		//re-align angle

		//if (speed>MAX_SPEED) speed=MAX_SPEED;

		car->real_x += ( car->real_speed * COS[ car->travel_ang >> 8 ]  ) >>8 ;
		car->real_y += ( car->real_speed * SIN[ car->travel_ang >> 8 ]  ) >>8 ;
		
		//speed-=30;
		if (car->real_speed>0) 
		{
			//slow down
			if ( car->real_speed < SLOWDOWN )
				car->real_speed-=1;
			else
				car->real_speed -= car->real_speed/ SLOWDOWN ;
		} else {
			//slow down, but, faster, cos we're going backwards
			if (car->real_speed > - SLOWDOWN )
				car->real_speed+=1;
			else
				car->real_speed -= car->real_speed/ SLOWDOWN ;
		}
			
	}

	//update the relavant sprite data
	hide_sprite( car->sprite );

	if ( (car->real_x>>8) - CAM_SCROLL_PIXEL_X >= -CAR_W )
		if ( ( (car->real_x>>8) - CAM_SCROLL_PIXEL_X) < (240) )
			if ( (car->real_y>>8) - CAM_SCROLL_PIXEL_Y >= -CAR_H )
				if ( ( (car->real_y>>8) - CAM_SCROLL_PIXEL_Y) < (160) )
				{
					move_sprite( car->sprite, (car->real_x>>8)-CAM_SCROLL_PIXEL_X , (car->real_y>>8)- CAM_SCROLL_PIXEL_Y );
					rotate_sprite( car->sprite, car->travel_ang>>8, ZOOM(1), ZOOM(1));
				}

}





void init_sprite(a_sprite *sprite,s16 tile_start_x , s16 tile_start_y , byte sprite_no, s16 angle)
{
	sprite->car_id=0;

	sprite->travel_ang=sprite->driving_ang=sprite->steering_ang= angle << 8;
	
	sprite->real_speed=0;
	sprite->real_x= (tile_start_x*TILE_W) << 8 ;
	sprite->real_y= (tile_start_y*TILE_W) << 8 ;
	sprite->sprite_no=sprite_no;

	sprites[sprite_no].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | 0;  
	sprites[sprite_no].attribute1 = SIZE_16 | ROTDATA(sprite_no) | 0;
	sprites[sprite_no].attribute2 = 0 | PRIORITY(3) ;
}






void update_sprite_gfx ( a_sprite *sprite, s32 view_x, s32 view_y )
{
	if ( sprite->real_x >>8 > view_x + 320 || sprite->real_y >>8 > view_y + 280 )
	{
		//move off screen
	}

	MoveSprite( sprite, (sprite->real_x >>8) - ( view_x ) , (sprite->real_y >>8) - ( view_y ) );
	RotateSprite( sprite , ZOOM(1) , ZOOM(1) );
}

*/