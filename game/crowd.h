#ifndef GAME_CROWD_H
#define GAME_CROWD_H

#include "../core/gba.h"
#include "../core/types.h"
#include "../media/map/map_data.h"


extern u8 crowd_mexwave_ticker[ MEDIA_MAP_W*MEDIA_MAP_H];

//note to self, swap shared BG's from 2&3 to 1&2. 1&2 is least used, 3 is most full
#define FRAME_COUNT				4
#define CROWD_CHAR_COUNT		3
#define ARROW_TILE_OFFSET		11//first arrow (pointing down) in the rink tile set
#define MEX_WAVE_PEAK			(6*FRAME_SCALE)
#define MEX_WAVE_DROP			(MEX_WAVE_PEAK/2)
#define CROWD_BODY_BG			FLOOR_BG
#define CROWD_HEAD_BG			OVERHEAD_BG
#define CROWD_HEAD				0
#define CROWD_BODY				1
#define FRAME_SCALE				1 //frame rate of "animation"'s

#define VALID_CROWD_MEM(m)		( m!=0 )
#define IS_CROWD_MEM(x,y)		( VALID_CROWD_MEM(CROWD_MEM_CHAR(x,y)+1) )
#define IS_CROWD_MEM_A(a)		( VALID_CROWD_MEM(CROWD_MEM_CHAR_A(a)+1) )
#define CROWD_MEM_FRAME(x,y)	( crowd_mem_frame_on_mex_wav[ MEX_WAVE_TICK(XY_ARRAY(x,y,MEDIA_MAP_W) / FRAME_SCALE ] )
#define CROWD_MEM_FRAME_A(a)	( crowd_mem_frame_on_mex_wav[ MEX_WAVE_TICK_A(a) / FRAME_SCALE ] )
#define CROWD_MEM_ANG(x,y)		( CROWD_MEM_ANG_A(XY_ARRAY(x,y,MEDIA_MAP_W) )
#define CROWD_MEM_ANG_A(a)		( (rink.floor_tiles[a]) - ARROW_TILE_OFFSET )
#define CROWD_MEM_CHAR(x,y)		( CROWD_MEM_CHAR_A(XY_ARRAY(x,y,MEDIA_MAP_W) )
#define CROWD_MEM_CHAR_A(a)		( crowd_char_map[a]-1 )
#define CROWD_TILE_N(ang,charn,frame) ( 0+ ( (ang-(charn)) *FRAME_COUNT) + (charn*FRAME_COUNT) + (frame)  )
#define MEX_WAVE_TICK(x,y)		( MEX_WAVE_TICK_A(XY_ARRAY(x,y,MEDIA_MAP_W)) )
#define MEX_WAVE_TICK_A(a)		( crowd_mexwave_ticker[a] )

#define MAX_CROWD_MAP_A			( MEDIA_MAP_W*MEDIA_MAP_H )

#define CROWD_ROW_UPDATE_MAX VISIBLE_TILE_H
#define CROWD_COLUMN_PER_ROW_UPDATE_RATE 3


void update_crowd_column(s16 column, u8 mex_wave_loop);
void update_crowd_row(s16 row, u8 mex_wave_loop, u8 loop_offset) ;
void broadcast_mexwav(u8 root_x,u8 root_y,u8 root_val);



#endif

