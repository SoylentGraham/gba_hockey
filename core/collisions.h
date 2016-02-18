#ifndef CORE_COLLISIONS_H
#define CORE_COLLISIONS_H


#include "gba.h"
#include "types.h"
#include "car.h"



//collision locations
#define COL_NONE BIT(0)
/*#define COL_FRONT BIT(1)
#define COL_BACK BIT(2)
#define COL_LEFT BIT(3)
#define COL_RIGHT BIT(4)*/
//wrong =/
#define COL_FRONT 1
#define COL_BACK 2
#define COL_LEFT 8
#define COL_RIGHT 4

#define COL_N 1
#define COL_S 2
#define COL_W 8
#define COL_E 4

#define COL_NE (COL_N | COL_E) //5
#define COL_NW (COL_N | COL_W) //9

#define COL_SE (COL_S | COL_E) //6
#define COL_SW (COL_S | COL_W) //10



//mask sizes
#define CAR_MASK_COUNT 360/5
#define CAR_MASK_W 16
#define CAR_MASK_H 16
#define TILE_MASK_MODE 0x0



//extern const u16 car_mask [ CAR_MASK_COUNT ] [ CAR_MASK_W * CAR_MASK_H ];
//extern const u16 tile_mask[ TILE_W * TILE_H ]; //all collidable

#define CAR_MASK( n ) ((u8*)car_mask + sizeof(u8)* ( ( (n - (n&1)) *256) + ( 16 * (n&1) ) ) )
//#define CAR_MASK( n ) ( (u16*)car_mask[ XY_ARRAY(0,n*8,TILE_W) ] )





u8 check_tiles_around_car( u8 *collision_id_a, u8 *collision_id_b,  s16 *collided_tile_x, s16 *collided_tile_y, a_car *car, u8 radius);

u8 check_collisions ( u8 width_a, u8 width_b, u8 height_a, u8 height_b,	 const u8 *mask_a, const u8 *mask_b, s16 x_coord_a, s16 y_coord_a, s16 x_coord_b, s16 y_coord_b, u8 *crash_id_a, u8 *crash_id_b, const u8 *u8_mask_a, const u8 *u8_mask_b  ) ;


#endif

