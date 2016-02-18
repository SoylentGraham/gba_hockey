#ifndef CORE_SPRITES_H
#define CORE_SPRITES_H



#include "gba.h"



#define MAX_SPRITES 128


//Atribute0 stuff
#define ROTATION_FLAG 		0x100
#define SIZE_DOUBLE			0x200
#define MODE_NORMAL     	0x0
#define MODE_TRANSPERANT	0x400
#define MODE_WINDOWED		0x800
#define MOSAIC				0x1000
#define COLOR_16			0x0000
#define COLOR_256			0x2000
#define SQUARE			0x0
#define WIDE			0x4000
#define TALL			0x8000

//Atribute1 stuff
#define ROTDATA(n)		((n) << 9)
#define HORIZONTAL_FLIP		0x1000
#define VERTICAL_FLIP		0x2000


#define SIZE_8			0x0
#define SIZE_16			0x4000
#define SIZE_32			0x8000
#define SIZE_64			0xC000


#define SLOWDOWN 40 //rate
#define SPEED_UP 40 //accel
#define SPEED_DOWN 20 //reverse
#define TURN_SLOW_DOWN 100 //rate
#define STEER_NONE 0
#define STEER_LEFT 1
#define STEER_RIGHT 2

#define ACCEL_NONE 0
#define ACCEL_FOWARD 1
#define ACCEL_BACKWARD 2

#define MAX_STEER 70 //degrees
#define DE_STEER 10 //modifed by speed factor




typedef struct {
	u16 attribute0;
	u16 attribute1;
	u16 attribute2;
	u16 attribute3;
} a_sprite;

//use these for casting to extract elemnts like x/y
typedef struct {
	u16 
		y: 8,
		rot: 1,
		sz_double:1,
		mode: 2,
		mosaic:1,
		col: 1,
		shape:2;
} a_sprite_attrib_0;

typedef struct {
	u16 
		x: 9,
		rot_id: 3,
		h_flip:1,
		v_flip: 1,
		size:2;
} a_sprite_attrib_1;


typedef struct {
	u16 
		character: 10,
		priority: 2,
		palette_number: 4;
} a_sprite_attrib_2;


typedef struct {
	u16 filler1[3];
	u16 pa;

	u16 filler2[3];
	u16 pb;	
		
	u16 filler3[3];
	u16 pc;	

	u16 filler4[3];
	u16 pd;
} a_sprite_rot;




//declars
/*
u16* OAM;
a_sprite sprites[128];
a_sprite_rot *sprite_rotate_data;
*/



void rotate_sprite(a_sprite *sprite, u16 angle, s16 x_scale,s16 y_scale);
void init_sprites(void);
void hide_sprite(a_sprite *sprite);
void update_sprites(void);
void move_sprite( a_sprite *sprite, s16 x, s16 y);

void init_sprite( a_sprite *sprite, u16 size, u16 orientaion, u8 obj_set, u8 obj_tile, u8 priority, u8 rot_id, u16 flip, u16 mirror );
void get_sprite_xy( a_sprite *sprite, s16 *x, s16*y);
void get_sprite_rot_id( a_sprite *sprite, u8 *rot_id);
u16 get_sprite_tile( a_sprite *sprite);

void flip_sprite( a_sprite *sprite, u8 flip_or_not );
void mirror_sprite( a_sprite *sprite, u8 mirror_or_not );

void set_sprite_tile(a_sprite *sprite, u16 tile);


#endif
