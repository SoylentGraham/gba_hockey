#ifndef GAME_RINK_MAP_H
#define GAME_RINK_MAP_H


#include "../core/gba.h"
#include "rink.h"



typedef struct {
	u16 *screen_tiles;
	u16 *above_tiles;
	u16 *floor_tiles;
	u16 *mask;
	u8 tileset[4];
} a_rink_map_data;


//global data
extern a_rink_map_data rink;

extern s16 camera_tiny_x_scroll;
extern s16 camera_tiny_y_scroll;
extern s16 camera_x_scroll;
extern s16 camera_y_scroll;




//#define RINK_CAMERA_X ( (rink_get_camera_x()>>8) ) 
//#define RINK_CAMERA_Y ( (rink_get_camera_y()>>8) ) 
#define RINK_CAMERA_X ( (rink_get_camera_x()) ) 
#define RINK_CAMERA_Y ( (rink_get_camera_y()) ) 

#define RINK_CAMERA_TILE_X (s16)( RINK_CAMERA_X /8 ) //left corner tile
#define RINK_CAMERA_TILE_Y (s16)( RINK_CAMERA_Y /8 ) //top corner tile

#define CAM_SCROLL_TILE_X (camera_x_scroll)
#define CAM_SCROLL_TILE_Y (camera_y_scroll)
#define CAM_SCROLL_PIXEL_X ((camera_x_scroll*8)+camera_tiny_x_scroll)
#define CAM_SCROLL_PIXEL_Y ((camera_y_scroll*8)+camera_tiny_y_scroll)


#define GOAL_W 6
#define GOAL_H 3
#define GOAL_L ((RINK_W/2)-(GOAL_W/2))
//tops of the goal's sprite
#define GOAL_T_A (RINK_EDGE_WIDTH+3+0) 
#define GOAL_T_B (RINK_H-RINK_EDGE_WIDTH-3-3)

#define GOAL_CENTER_X_A ((GOAL_L + (GOAL_W/2))*TILE_W)
#define GOAL_CENTER_X_B ((((GOAL_L + (GOAL_W/2))-3)*TILE_W))

#define GOAL_CENTER_Y_A ((GOAL_T_A + 2)*TILE_H)
#define GOAL_CENTER_Y_B ((GOAL_T_B + 1)*TILE_H)

#define REAL_GOAL_CENTER_X(t) (real_goal_center[t][X])
#define REAL_GOAL_CENTER_Y(t) (real_goal_center[t][Y])

static const real_goal_center[2][2] = {
	GOAL_CENTER_X_A<<8,
	GOAL_CENTER_Y_A<<8,
	GOAL_CENTER_X_B<<8,
	GOAL_CENTER_Y_B<<8,
};



#define RELOAD_RINK_ROW(sx,sy,mx,my) rink_reload_row(sx,sy,mx,my,	0,	rink.screen_tiles,	rink.above_tiles,	rink.floor_tiles,	RINK_W,RINK_H);
#define RELOAD_RINK_COL(sx,sy,mx,my) rink_reload_column(sx,sy,mx,my,0,	rink.screen_tiles,	rink.above_tiles,	rink.floor_tiles,	RINK_W,RINK_H);

#define RELOAD_RINK(sx,sy) rink_reset(sx,sy)




//void rink_reload_bg_tiles_at( u16 sx, u16 sy, u16 mx, u16 my, u8 *data_map_a, u8 *data_map_b, u8 *data_map_c, u8 *data_map_d, u8 data_w, u8 data_h );
void rink_reset(s16 x_start, s16 y_start);
void rink_reload_row(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y, u16 *data_map_a, u16 *data_map_b, u16 *data_map_c, u16 *data_map_d, u8 data_w, u8 data_h );
void rink_reload_column(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y, u16 *data_map_a, u16 *data_map_b, u16 *data_map_c, u16 *data_map_d, u8 data_w, u8 data_h );


u8 coord_visible(s32 x, s32 y, s16 obj_w, s16 obj_h);

void rink_release_camera(void);
void rink_lock_camera(void);
void update_free_cam(void);




#endif

