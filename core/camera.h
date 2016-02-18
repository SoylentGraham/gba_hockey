#ifndef CAMERA_H
#define CAMERA_H


#include "gba.h"
#include "car.h"
#include "types.h"



#define TINY_SCROLL_AMOUNT TILE_W



//#define CAMERA_X (s16)( ( (*real_camera[X]) >> 8 ) - (VISIBLE_SCREEN_W/2) ) //left corner
//#define CAMERA_Y (s16)( ( (*real_camera[Y]) >> 8 ) - (VISIBLE_SCREEN_H/2) ) //top corner
#define CAMERA_X ( get_camera_x() ) 
#define CAMERA_Y ( get_camera_y() ) 

#define CAMERA_TILE_X (s16)( CAMERA_X /8 ) //left corner tile
#define CAMERA_TILE_Y (s16)( CAMERA_Y /8 ) //top corner tile

#define CAM_SCROLL_TILE_X (camera_x_scroll)
#define CAM_SCROLL_TILE_Y (camera_y_scroll)
#define CAM_SCROLL_PIXEL_X ((camera_x_scroll*8)+camera_tiny_x_scroll)
#define CAM_SCROLL_PIXEL_Y ((camera_y_scroll*8)+camera_tiny_y_scroll)


s32 real_camera_save[2];
s32 *real_camera[2];


s16 get_camera_x(void);
s16 get_camera_y(void);
void bind_camera(a_car *car);
void lock_camera(void);
void release_camera(void) ;
void unbind_camera(void);



#endif

