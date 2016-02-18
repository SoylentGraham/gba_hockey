#include "gba.h"
#include "types.h"
#include "math.h"
#include "dma.h"
#include "core.h"
#include "sprites.h"
#include "tile_alloc.h"




/*
u32* OAMmem = (u32*)0x7000000;
u16* VideoBuffer = (u16*)0x6000000;
u16* OAMData = (u16*)0x6010000;
u16* BGPaletteMem = (u16*)0x5000000;
//sprite palette addr
u16* OBJPaletteMem = (u16*)0x5000200;
*/

//where our tiles are store
u16* oam_data = (volatile)(u16*)0x6010000;

//oam addr for sprite data
u16 *OAM = (volatile)(u16*)0x7000000;
//local copy so we dont update in the vblank ors omething
a_sprite sprites[MAX_SPRITES];
//a_sprite_rot *sprite_rotate_data = sprites;
a_sprite_rot *sprite_rotate_data = ((a_sprite_rot*)sprites);


u16 get_sprite_tile( a_sprite *sprite)
{
	return (((a_sprite_attrib_2*)&sprite->attribute2)->character)/2;
}


void get_sprite_xy( a_sprite *sprite, s16 *x, s16*y)
{
	if (!x && !y) return;

	if (x) *x=((a_sprite_attrib_1*)&sprite->attribute1)->x;
	if (y) *y=((a_sprite_attrib_0*)&sprite->attribute0)->y;

	if ( sprite->attribute1 & SIZE_16 && sprite->attribute1 & SIZE_32 )	{
		if (x) *x+=32;
		if (y) *y+=32;
	}
	else
	if ( sprite->attribute1 & SIZE_16 )	{
		if (x) *x+=8;
		if (y) *y+=8;
	} 
	else
	if ( sprite->attribute1 & SIZE_32 )	{
		if (x) *x+=16;
		if (y) *y+=16;
	}
	else //must be size_8 which is 0x0
	{
		if (x) *x+=4;
		if (y) *y+=4;
	}

	if (x) *x = roll_s16( *x, 0, 512 );
	if (y) *y = roll_s16( *y, 0, 256 );

}


void get_sprite_rot_id( a_sprite *sprite, u8 *rot_id)
{
	*rot_id=((a_sprite_attrib_1*)&sprite->attribute1)->rot_id;
}


void set_sprite_tile(a_sprite *sprite, u16 tile)
{
	u16 pri;
	pri = ((a_sprite_attrib_2*)&sprite->attribute2)->priority;
	sprite->attribute2 = tile ;
	sprite->attribute2 |= PRIORITY(pri) ;

}



void move_sprite( a_sprite *sprite, s16 x, s16 y)
{
	//OAMEntry* sp = &sprites[ sprite->sprite_no ];

	x = roll_s16( x, 0, 512 );
	y = roll_s16( y, 0, 256 );

//	if ( sprite->attribute1 & SIZE_64 )	{
	if ( sprite->attribute1 & SIZE_16 && sprite->attribute1 & SIZE_32 )	{
		x-=32; y-=32;
	}
	else
	if ( sprite->attribute1 & SIZE_16 )	{
		x-=8; y-=8;
	} 
	else
	if ( sprite->attribute1 & SIZE_32 )	{
		x-=16; y-=16;
	}
	else //must be size_8 which is 0x0
	{
		x-=4; y-=4;
	}

	if(x < 0)	x = 512 + x;
	if(y < 0)	y = 256 + y;

	sprite->attribute1 = sprite->attribute1 & 0xFE00;  //clear the old x value
	sprite->attribute1 = sprite->attribute1 | x;
	
	sprite->attribute0 = sprite->attribute0 & 0xFF00;  //clear the old y value
	sprite->attribute0 = sprite->attribute0 | y;
}


void update_sprites(void) //copies oam data
{
	DMACopyU16( sprites, OAM, MAX_SPRITES*4, 1 );
/*	
	u16 Loop;
	for(loop = 0; loop < MAX_SPRITES*4; loop++) // *4 attrib u16s
	{
		OAM[loop] = ((u16*)sprites)[loop];
	}
	*/
}


void hide_sprite(a_sprite *sprite)
{
	//anywhere offscreen
	move_sprite(sprite,160,240);
	//move_sprite(sprite,0,0);
}


void flip_sprite( a_sprite *sprite, u8 flip_or_not )
{
	if ( !flip_or_not )
		((a_sprite_attrib_1*)&sprite->attribute1)->v_flip = 0;
	else
		((a_sprite_attrib_1*)&sprite->attribute1)->v_flip = 1;

}

void mirror_sprite( a_sprite *sprite, u8 mirror_or_not )
{	
	if ( !mirror_or_not )
		((a_sprite_attrib_1*)&sprite->attribute1)->h_flip = 0;
	else
		((a_sprite_attrib_1*)&sprite->attribute1)->h_flip = 1;
}



void init_sprite( a_sprite *sprite, u16 size, u16 orientaion, u8 obj_set, u8 obj_tile, u8 priority, u8 rot_id, u16 flip, u16 mirror )
{
//	sprite->attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | 0;
	sprite->attribute0 = orientaion;
//	sprite->attribute0 = orientaion;
	sprite->attribute0 |= COLOR_256 ;

	sprite->attribute1 = size ;
//	sprite->attribute1 |= ROTDATA(rot_id) ;
	sprite->attribute1 |= 0 ;
//	sprite->attribute2 = OBJ_TILE(obj_set,obj_tile) ;
	sprite->attribute2 = (((bg_tile_set[OBJ][obj_set])*2)+obj_tile);
	sprite->attribute2 |= PRIORITY(priority) ;
	//draw_text(0,5,true,"%d",OBJ_TILE(obj_set,obj_tile));

	mirror_sprite( sprite, mirror);
	
	flip_sprite( sprite, flip);
}


void init_sprites(void)
{
	u16 i;
	for( i=0; i< 128; i++)
	{
		hide_sprite( &sprites[i] );
		rotate_sprite(&sprites[i], 0, ZOOM(1) , ZOOM(1) );
	}
}


//	for(loop = 0; loop < 256; loop++)
//		OBJPaletteMem[loop] = gfx_palette[loop];	//loop through and store the palette from your pict 



/*
	FIXED pa,pb,pc,pd;
	u16 angle;
	angle = roll_u16( (sprite->travel_ang>>8) +90 ,0,360 );
       
	pa = ((x_scale) * COS[ angle ])>>8; //(do my fixed point multiplies and shift back down)
	pb = ((y_scale) * SIN[ angle ])>>8;
	pc = ((x_scale) * -SIN[ angle ])>>8;
	pd = ((y_scale) * COS[ angle ])>>8;


	rotData[ sprite->sprite_no ].pa = pa; //put them in my data struct
	rotData[ sprite->sprite_no ].pb = pb;
	rotData[ sprite->sprite_no ].pc = pc;
	rotData[ sprite->sprite_no ].pd = pd;
	
}
*/


void rotate_sprite(a_sprite *sprite, u16 angle, s16 x_scale,s16 y_scale)
{
	s16 pa,pb,pc,pd;
	//a_sprite_rot *rot_sprite = (*a_sprite_rot)sprite;
	u8 rot_n;
	get_sprite_rot_id(sprite, &rot_n);
	a_sprite_rot *rot_sprite = &sprite_rotate_data[rot_n];

	angle = roll_u16( angle +90 ,0,360 );
       
	//do my fixed point multiplies and shift back down
	pa = ((x_scale) * COS[ angle ])>>8; 
	pb = ((y_scale) * SIN[ angle ])>>8;
	pc = ((x_scale) * -SIN[ angle ])>>8;
	pd = ((y_scale) * COS[ angle ])>>8;


	rot_sprite->pa = pa;
	rot_sprite->pb = pb;
	rot_sprite->pc = pc;
	rot_sprite->pd = pd;
}





/*


  #define LIGHT_OVERLAP 0 //should be even
#define LEFT_LIGHT 1 //oam offsets
#define RIGHT_LIGHT 2 
#define LIGHT_YELLOW 0 //char number for the sprite
#define LIGHT_DEVIANT 2 //make it shine off at an angle

void set_sprite_lights(a_sprite *sprite)
{
	//dealing with large numbers
	s32 sprite_center[2]; //x/y
	s32 light_tops[2]; //foward of the car
	s32 left_light[2]; //sprite center for the light on the left
	s32 right_light[2]; // " " and right
	
	//pre-calc these angles
	s16 angle = sprite->car_direction>>8;
	s16 angle_less = roll(angle - 90, 0, 360);
	s16 angle_more = roll(angle - 90, 0, 360);

	//need to work from center for correct calcs
	sprite_center[X] = sprite->real_x + ( (sprite_w<<8) /2);
	sprite_center[Y] = sprite->real_y + ( (sprite_h<<8) /2);

	//these lengths dont (?) require shifitng 'cos the cos/sin is already big
	light_tops[X] = sprite_center[X] + ( cos[ angle ] * ( (SPRITE_H/2) + LIGHT_H ) );
	light_tops[Y] = sprite_center[Y] + ( sin[ angle ] * ( (SPRITE_H/2) + LIGHT_H ) );

	left_light[X] = light_tops[X] + ( cos[ angle_less ] * (LIGHT_W-LIGHT_OVERLAP) ) ;
	left_light[Y] = light_tops[Y] + ( sin[ angle_less ] * (LIGHT_W-LIGHT_OVERLAP) ) ;

	right_light[X] = light_tops[X] + ( cos[ angle_more ] * (-LIGHT_OVERLAP) ) ;
	right_light[Y] = light_tops[Y] + ( sin[ angle_more ] * (-LIGHT_OVERLAP) ) ;

	//update the oam data, although prolly dont need a seperate func
	set_light( oam[sprite->sprite_no + LEFT_LIGHT],  left_light, car_ang - LIGHT_DEVIANT , LIGHT_YELLOW );
	set_light( oam[sprite->sprite_no + RIGHT_LIGHT],  right_light, car_ang + LIGHT_DEVIANT , LIGHT_YELLOW );
}


  */