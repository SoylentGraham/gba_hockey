#include "../core/gba.h"
#include "../core/tile_alloc.h"
#include "../core/types.h"
#include "scoreboard.h"
#include "tile_sets.h"
#include "game.h"

#define DISABLE_SCORE_SCROLL 1
#define SCOREBOARD_ANIM_DELAY 5
//#define GRID_NUMBER(n) ((u8*)score_board_numbers[n*SCOREBOARD_NUMBER_SIZE])
#define GRID_NUM_POS(n) ( (n*GRID_NUM_H)*GRID_NUM_W )
#define GRID_NUM_W (6/2) //4 in pixels 2 tiles
#define GRID_NUM_H (6/2) //6 pix, 3 tile
#define SCOREBOARD_NUMBER_SIZE (GRID_NUM_W*GRID_NUM_H)
#define SHOWING_CUSTOM_SCOREBOARD ((void*)current_scoreboard_anim == (void*)scoreboard_anims[SCOREBOARD_CUSTOM] )

u8 *current_scoreboard_anim;
extern u8 game_score[2];



u16 score_board_frame = 0;
u8 custom_needs_updating[4] = {false};
#define ANY_CUSTOM_NEEDS_UPDATE (custom_needs_updating[0]||custom_needs_updating[1]||custom_needs_updating[2]||custom_needs_updating[3] )
u8 custom_grid_number[4];

const grid_num_left_offset[4] = { GRID_NUM_W*0, GRID_NUM_W*1, GRID_NUM_W*2, GRID_NUM_W*3, };



#define SCOREBOARD_ANIM_LENGTH( anim ) ( sizeof(anim) / (sizeof(u8)*(SCOREBOARD_W*SCOREBOARD_H)) )



const u8 score_board_numbers[ 10*SCOREBOARD_NUMBER_SIZE ] = {
  8, 12, 12,  10,  5, 15, 10, 13, 15,  
  0, 12,  4,  0,  10,  5,  0, 10,  5, 
  8, 12, 12,  8,  12, 15, 10, 13, 12,  
  8, 12, 12,  8,  12, 15,  8, 12, 15,  
  8,  4, 12,  10,  5, 15,  2,  3, 15, 
  8, 12, 12,  10, 13, 12,  8, 12, 15,  
  8, 12,  4,  10, 13, 12, 10, 13, 15,  
  8, 12, 12,  0,  8,   7,  0, 15,  0, 
  8, 12, 12,  10, 13, 15, 10, 13, 15,  
  8, 12, 12,  10, 13, 15,  0, 12, 15,  


};

const u8 goal_blink_score_board[ ] = {
//u8 goal_blink_score_board[ 1 + (SCOREBOARD_W * SCOREBOARD_H) ] = {
//frame count
	37,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10, 10,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  2,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0, 12,
  0,  0,  0,  0,  0,  0,  0,  0,  0, 10, 10,  8,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  2, 12,

  0,  0,  0,  0,  0,  0,  0,  0,  8,  0, 12,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 10, 10,  8,  4,
  0,  0,  0,  0,  0,  0,  0,  0,  8,  2, 12,  1,



  0,  0,  0,  0,  0,  0,  0,  8,  0, 12,  0,  8,
  0,  0,  0,  0,  0,  0,  0, 10, 10,  8,  4,  5,
  0,  0,  0,  0,  0,  0,  0,  8,  2, 12,  1,  9,

  0,  0,  0,  0,  0,  0,  8,  0, 12,  0,  8,  4,
  0,  0,  0,  0,  0,  0, 10, 10,  8,  4,  5, 10,
  0,  0,  0,  0,  0,  0,  8,  2, 12,  1,  9,  6,

  0,  0,  0,  0,  0,  8,  0, 12,  0,  8,  4,  0,
  0,  0,  0,  0,  0, 10, 10,  8,  4,  5, 10, 10,
  0,  0,  0,  0,  0,  8,  2, 12,  1,  9,  6, 10,

  0,  0,  0,  0,  8,  0, 12,  0,  8,  4,  0, 12,
  0,  0,  0,  0, 10, 10,  8,  4,  5, 10, 10, 12,
  0,  0,  0,  0,  8,  2, 12,  1,  9,  6, 10,  0,

  0,  0,  0,  8,  0, 12,  0,  8,  4,  0, 12,  0,
  0,  0,  0, 10, 10,  8,  4,  5, 10, 10, 12,  5,
  0,  0,  0,  8,  2, 12,  1,  9,  6, 10, 0,  5,



  0,  0,  8,  0, 12,  0,  8,  4,  0, 12,  0,  4,
  0,  0, 10, 10,  8,  4,  5, 10, 10, 12,  5,  5,
  0,  0,  8,  2, 12,  1,  9,  6, 10,  0,  5, 13,

  0,  8,  0, 12,  0,  8,  4,  0, 12,  0,  4,  0,
  0, 10, 10,  8,  4,  5, 10, 10, 12,  5,  5,  0,
  0,  8,  2, 12,  1,  9,  6, 10,  0,  5, 13,  4,

  8,  0, 12,  0,  8,  4,  0, 12,  0,  4,  0,  4,
 10, 10,  8,  4,  5, 10, 10, 12,  5,  5,  0,  5,
  8,  2, 12,  1,  9,  6, 10,  0,  5, 13,  4,  4,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  8,  0, 12,  0,  8,  4,  0, 12,  0,  4,  0,  4,
 10, 10,  8,  4,  5, 10, 10, 12,  5,  5,  0,  5,
  8,  2, 12,  1,  9,  6, 10,  0,  5, 13,  4,  4,



  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  8,  0, 12,  0,  8,  4,  0, 12,  0,  4,  0,  4,
 10, 10,  8,  4,  5, 10, 10, 12,  5,  5,  0,  5,
  8,  2, 12,  1,  9,  6, 10,  0,  5, 13,  4,  4,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  8,  0, 12,  0,  8,  4,  0, 12,  0,  4,  0,  4,
 10, 10,  8,  4,  5, 10, 10, 12,  5,  5,  0,  5,
  8,  2, 12,  1,  9,  6, 10,  0,  5, 13,  4,  4,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

//20

  8,  0, 12,  0,  8,  4,  0, 12,  0,  4,  0,  4,
 10, 10,  8,  4,  5, 10, 10, 12,  5,  5,  0,  5,
  8,  2, 12,  1,  9,  6, 10,  0,  5, 13,  4,  4,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  8,  0, 12,  0,  8,  4,  0, 12,  0,  4,  0,  4,
 10, 10,  8,  4,  5, 10, 10, 12,  5,  5,  0,  5,
  8,  2, 12,  1,  9,  6, 10,  0,  5, 13,  4,  4,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,




  15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,

  15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,

  1,  1,  2,  2,  4,  2,  0,  2,  4,  2,  0,  9,
  1,  4,  1,  8,  8,  8,  2,  0,  4,  1,  1,  8,
  1,  2,  8,  0,  8,  4,  8,  2,  4,  9,  4,  0,

    1,  1,  2,  2,  4,  2,  0,  2,  4,  2,  0,  9,
  1,  4,  1,  8,  8,  8,  2,  0,  4,  1,  1,  8,
  1,  2,  8,  0,  8,  4,  8,  2,  4,  9,  4,  0,

  0,  1,  0,  0,  0,  0,  0,  0,  0,  2,  1,  2,
  1,  1,  1,  0,  0,  0,  0,  0,  4,  1,  6,  4,
  1,  6,  0,  0,  0,  0,  0,  0,  0,  4,  4,  8,



  0,  1,  0,  0,  0,  0,  0,  0,  0,  2,  1,  2,
  1,  1,  1,  0,  0,  0,  0,  0,  4,  1,  6,  4,
  1,  6,  0,  0,  0,  0,  0,  0,  0,  4,  4,  8,

  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  8,  0,  0,  0,  0,  0,  0,  0,  0,  8,  2,

  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  8,  0,  0,  0,  0,  0,  0,  0,  0,  8,  2,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

};

  


u8 custom_scoreboard_buffer[ (SCOREBOARD_W * SCOREBOARD_H) + 1] = {0};


const u8 blank_scoreboard_frame[ ( SCOREBOARD_W * SCOREBOARD_H ) + 1] = {
	1, //frame count
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};


#define BLANK_SCOREBOARD_SPACE -1
#define BS BLANK_SCOREBOARD_SPACE


const u8 score_board_outline[ (SCOREBOARD_W+2) * (SCOREBOARD_H+2) ] = {
	0,	1,	1,	8,	1,	1,	1,	1,	1,	1,	8,	1,	1,	2,
	3,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	4,
	3,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	4,
	3,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	BS,	4,
	5,	6,	6,	9,	10,	11,	6,	6,	6,	12,	13,	14,	6,	7,
};



const u8 scoreboard_lengths[SCOREBOARD_ANIM_COUNT] = {
	SCOREBOARD_ANIM_LENGTH(blank_scoreboard_frame),
	SCOREBOARD_ANIM_LENGTH(goal_blink_score_board),
	SCOREBOARD_ANIM_LENGTH(custom_scoreboard_buffer),
};

const u8 *scoreboard_anims[SCOREBOARD_ANIM_COUNT] ={
	blank_scoreboard_frame,
	goal_blink_score_board,
	custom_scoreboard_buffer,
};











void set_scoreboard_anim(u8 anim_id)
{
	current_scoreboard_anim = (u8*)scoreboard_anims[ anim_id ];
	score_board_frame = 0;
}


void update_custom_scoreboard_numbers(u8 score_a, u8 score_b)
{
	u8 g,n,x,y,a;
	u8 old_num[4];

	old_num[0] = custom_grid_number[0];
	old_num[1] = custom_grid_number[1];
	old_num[2] = custom_grid_number[2];
	old_num[3] = custom_grid_number[3];

	custom_grid_number[1] = score_a%10;
	custom_grid_number[0] = (score_a - custom_grid_number[1])/10;

	custom_grid_number[3] = score_b%10;
	custom_grid_number[2] = (score_b - custom_grid_number[3])/10;


	custom_needs_updating[0] = ( custom_grid_number[0] != old_num[0] );
	custom_needs_updating[1] = ( custom_grid_number[1] != old_num[1] );
	custom_needs_updating[2] = ( custom_grid_number[2] != old_num[2] );
	custom_needs_updating[3] = ( custom_grid_number[3] != old_num[3] );

	memset( custom_needs_updating, true, sizeof(u8) * 4 );

	//set anim length
	custom_scoreboard_buffer[0] = GRID_NUM_H;

	set_scoreboard_anim( SCOREBOARD_CUSTOM );
}







void draw_frame_row( u8 *output_frame, u16 draw_x, u16 draw_y )
{
	u16 x;
	x=0;
	while ( x < SCOREBOARD_W )
	{
	//	draw_tile( draw_x+x, draw_y, BG_TILE(SCORE_BOARD_LEDS) + output_frame[ x ] );
		draw_tile( draw_x+x, draw_y, BG_TILE(HUD_BG,SCOREBOARD_TILES) + output_frame[ x ], REAL_BG(HUD_BG) );
		x++;
	}
}

void draw_frame( u8 *output_frame )
{
	u8 y;

	for (y=0; y<SCOREBOARD_H; y++)
		draw_frame_row( &output_frame[ (SCOREBOARD_W)*y ], SCOREBOARD_LEFT, SCOREBOARD_TOP+y );
}



void update_custom_scoreboard(void)
{
	u8 g,n,x,y,a,val;
	u16 f;

	if (!ANY_CUSTOM_NEEDS_UPDATE) return;

	f = GRID_NUM_H - ( score_board_frame / SCOREBOARD_ANIM_DELAY );
	if (DISABLE_SCORE_SCROLL)
		f=0;
	
	for (x=0; x<GRID_NUM_W; x++ )
	{
		for ( y=0; y<GRID_NUM_H; y++ )
		{
			n = XY_ARRAY( x, y, GRID_NUM_W );
			a = XY_ARRAY( x, y, SCOREBOARD_W ) + 1;

			if ( custom_needs_updating[0]==true )
			{
				if (y<f)	val = 0;
				else		val = score_board_numbers[ n + GRID_NUM_POS(custom_grid_number[0]) ];
				custom_scoreboard_buffer[ a + grid_num_left_offset[0] ] =  val;
			}
	
			if ( custom_needs_updating[1]==true )
			{
				if (y<f)	val = 0;
				else		val = score_board_numbers[ n + GRID_NUM_POS(custom_grid_number[1]) ];
				custom_scoreboard_buffer[ a + grid_num_left_offset[1] ] =  val ;
			}
			
			if ( custom_needs_updating[2]==true )
			{
				if (y<f)	val = 0;
				else		val = score_board_numbers[ n + GRID_NUM_POS(custom_grid_number[2]) ];
				custom_scoreboard_buffer[ a + grid_num_left_offset[2] ] =  val ;
			}

			if ( custom_needs_updating[3]==true )
			{
				if (y<f)	val = 0;
				else		val = score_board_numbers[ n + GRID_NUM_POS(custom_grid_number[3]) ];
				custom_scoreboard_buffer[ a + grid_num_left_offset[3] ] =  val;
			}



		}
	}



}



void show_custom_scoreboard(u8 refresh)
{
	//regular set numbers, but makes all update
	update_custom_scoreboard_numbers( game_score[0], game_score[1] );
	memset( custom_needs_updating, refresh, sizeof(u8)*4 );
}


void update_scoreboard(void)
{
	u8 frame_count,frame_to_draw;

	frame_count = current_scoreboard_anim[0];
	
	//upate animation if changed
	if ( score_board_frame%SCOREBOARD_ANIM_DELAY == 0 )
	{
		frame_to_draw = score_board_frame / SCOREBOARD_ANIM_DELAY ;


		if ( SHOWING_CUSTOM_SCOREBOARD )
			frame_to_draw = 0;

		draw_frame( &current_scoreboard_anim[ ( (frame_to_draw)*(SCOREBOARD_W * SCOREBOARD_H))+1 ] );
	}

	update_custom_scoreboard();


	score_board_frame++;
	if ( score_board_frame > frame_count*SCOREBOARD_ANIM_DELAY  )
	{
		score_board_frame = 0;

		show_custom_scoreboard(!SHOWING_CUSTOM_SCOREBOARD);
		
	}
	

}





//blank, and draw inital tiles
void init_scoreboard(void)
{
	u8 x,y;

	//set_scoreboard_anim( SCOREBOARD_BLANK );
	set_scoreboard_anim( SCOREBOARD_CUSTOM );
	
	custom_needs_updating[0] = false;
	custom_needs_updating[1] = false;
	custom_needs_updating[2] = false;
	custom_needs_updating[3] = false;
	

	for (x=0;x<SCOREBOARD_W+2;x++)
	{
		for (y=0;y<SCOREBOARD_H+2;y++)
		{
			draw_tile( x, y, BG_TILE(HUD_BG,SCOREBOARD_TILES) + score_board_outline[ XY_ARRAY(x,y,(SCOREBOARD_W+2)) ] + 16, REAL_BG(HUD_BG) );
		}
	}


	update_custom_scoreboard_numbers( game_score[0], game_score[1] );
	show_custom_scoreboard(true);

	update_scoreboard();
}



































