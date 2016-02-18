#ifndef CORE_CAR_H
#define CORE_CAR_H


#include "gba.h"
#include "sprites.h"

#define AI_NONE 0
#define AI_AMBIENT 0
#define AI_COP 0


//things i can collide with
#define COLLIDE_NONE BIT(0)
#define COLLIDE_CAR BIT(1)
#define COLLIDE_OBJ BIT(2)
#define COLLIDE_BUILDING BIT(4)
#define COLLIDE_ALT_TILE BIT(8)
#define COLLIDE_ALL ((u8) (COLLIDE_CAR | COLLIDE_OBJ | COLLIDE_BUILDING | COLLIDE_ALT_TILE) )


#define MOVE_NONE BIT(0)
#define ACCEL_NONE BIT(0)
#define STEER_NONE BIT(0)

#define ACCEL_FOWARD BIT(1)
#define ACCEL_BACKWARD BIT(2)
#define STEER_LEFT BIT(4)
#define STEER_RIGHT BIT(8)
#define HANDBRAKE BIT(16)


typedef struct {
	//charcater data for sprite
	u8 sprite_tile_start;
	//speed of acceleration
	s32 accel_rate;
	//amount we can turn at once
	s32 turn_rate;
} a_car_type;




typedef struct {
//real_*'s need >>8 for pixel coords
	u8 ai_mode;
	//input bytes
	u16 movement; 
	//u8 last_steer_dir; 

	//these need shifting to pixel-relative vals
	s32 real_x; 
	s32 real_y;
	s32 real_speed;
	
	s32 steering_ang; //steering wheel pivot
	//shifting required after cos/sin use
	s32 driving_ang; //direction car is heading, + influence on travel
	s32 travel_ang; //actual travel

	u8 rot_id;
	a_sprite *sprite;
	//contains character number, accelaration/mass details etc
	a_car_type *car_type;
	u8 collision_data;
} a_car;



//extern a_car_type car_types[CAR_TYPE_COUNT];
//extern a_car *player_controlled_car;

void set_car_type( a_car_type *car_type_data, u8 sprite_tile_start, s32 accel_rate, s32 turn_rate );
void bind_player_car(a_car *players_car);
void update_players_car(void);
void init_car( a_car *car, a_sprite *sprite, a_car_type *car_type , s16 init_tile_x, s16 init_tile_y, u16 init_ang, u8 collide_data, u8 ai_mode);
void process_car(a_car *car);



#endif
