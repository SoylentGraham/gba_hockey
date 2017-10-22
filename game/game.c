#include "game.h"
#include "tile_sets.h"
#include "puck.h"
#include "rink.h"
#include "rink_map.h"
#include "main.h"
#include "scoreboard.h"
#include "hockey_sprites.h"
#include "crowd.h"

#include "../core/gba.h"
#include "../core/core.h"
#include "../core/types.h"
#include "../core/tile_alloc.h"
#include "../core/map.h"
#include "../core/math.h" //cos/sin
#include "../core/sprites.h"
#include "../core/animated_sprite.h"
#include "../core/keypad.h"
#include "../core/camera.h"
#include "../core/font.h"
#include "../core/collisions.h"

extern s16 camera_tiny_x_scroll;
extern s16 camera_tiny_y_scroll;
extern s16 camera_x_scroll;
extern s16 camera_y_scroll;
extern s32 current_camera_pos[2];

extern const u8 tile_set_lengths[TILE_TYPE_COUNT] ;
extern a_sprite sprites[MAX_SPRITES];
extern a_sprite_anim sprite_animations[MAX_SPRITES];
extern a_puck puck;

a_hockey_player team_players[TEAM_COUNT][PLAYER_COUNT/TEAM_COUNT];
//const a_hockey_player *players = (a_hockey_player*)team_players;
a_hockey_player *players = (a_hockey_player*)team_players;

//global quick ref to controlled p
a_hockey_player *local_controlled_player=0;


const u8 col_bit_angles[] = { COL_N, COL_NE, COL_E, COL_SE, COL_S, COL_SW, COL_W, COL_NW };

extern a_object_target targets[TARGET_MAX];


u16 press_a=0;
u16 press_b=0;
u16 press_start=0;
u8 active_player=0;
u8 game_score[2];
u8 game_mode=MODE_INIT;





//scoreboard, 50fps
//ai 40fps for 1up

u8 update_lineup(void);

u8 show_debug=false;
u8 press_select=false;


u8 sprite_count;
u8 sprite_anim_count;

extern u32 game_fps,fps_counter;

u16 game_countdown;


u8 on=0;
extern u16 *tile_map[4];
u8 crowd_row_counter=0;

#define GAME_MAX_BRIGHT 31
s16 game_brightness_mod; // 0 is plain

extern s16 tiny_scroll_offset_y[4];
extern const s16 tiny_scroll_offset_x[4];

#define REG_SRAM   ((volatile u8*)0xE000000)
//	get player count from SRAM




void update_game_brightness(void)
{
	load_tile_palette(-game_brightness_mod);
	
	if ( game_brightness_mod>0 )
	{
		game_brightness_mod--;
		tiny_scroll_offset_y[0]--;
		scrollbackground( tiny_scroll_offset_x[0], tiny_scroll_offset_y[0], HUD_BG );
	}
}


void init_game(void)
{
	u8 i,t,x,y;

	hide_all_backgrounds;
	set_mode(0, BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE ,1);

	//REG_BLDMOD = TARGET_TOP_BG0 | TARGET_TOP_BG2 | TARGET_LOW_BG2 | TARGET_LOW_BG1 | TARGET_LOW_BG3 | TARGET_LOW_OBJ | BLEND_NEG;
	REG_BLDMOD = TARGET_TOP_BG1 | TARGET_LOW_BG2 | TARGET_LOW_BG1 | TARGET_LOW_BG3 | TARGET_LOW_OBJ | BLEND_NEG;
	REG_COLEV = BLEND_LEVEL(8);

	clear_current_tiledata(BG(0));
	clear_current_tiledata(BG(1));
	clear_current_tiledata(BG(2));
	clear_current_tiledata(BG(3));
	clear_current_tiledata(OBJ);
	
	game_brightness_mod = GAME_MAX_BRIGHT;
	tiny_scroll_offset_y[0] = GAME_MAX_BRIGHT;
	update_game_brightness();

	//tiles i need for my hud (HUD_BG)
	load_tile_set(BG(HUD_BG),	BLANK_TILES);
	load_tile_set(BG(HUD_BG),	FONT_TILES);
	load_tile_set(BG(HUD_BG),	SCOREBOARD_TILES);	

	load_tile_set(BG(SCREEN_BG),	BLANK_TILES);
	load_tile_set(BG(SCREEN_BG),	ICE_RINK_SCREEN_TILES);

	load_tile_set(BG(OVERHEAD_BG),	BLANK_TILES);
	load_tile_set(BG(OVERHEAD_BG),	ICE_RINK_OVERHEAD_TILES);
	load_tile_set(BG(OVERHEAD_BG),	CROWD_CHAR1_HEAD_TILES);
	load_tile_set(BG(OVERHEAD_BG),	CROWD_CHAR2_HEAD_TILES);
	load_tile_set(BG(OVERHEAD_BG),	CROWD_CHAR3_HEAD_TILES);

	load_tile_set(BG(FLOOR_BG),	BLANK_TILES);
	load_tile_set(BG(FLOOR_BG),	ICE_RINK_FLOOR_TILES);
	load_tile_set(BG(FLOOR_BG),	CROWD_CHAR1_BODY_TILES);
	load_tile_set(BG(FLOOR_BG),	CROWD_CHAR2_BODY_TILES);
//	load_tile_set(BG(FLOOR_BG),	CROWD_CHAR3_BODY_TILES);
	
	//tiles for sprites
	load_tile_set(OBJ,	FONT_TILES);
	load_tile_set(OBJ,	HOCKEY_GUY_HEAD_TILES);
	load_tile_set(OBJ,	HOCKEY_GUY_BODY_TILES);
	load_tile_set(OBJ,	PUCK_TILES);
	load_tile_set(OBJ,	TARGET_TILES);
	load_tile_set(OBJ,	ARROW_TILES);
	load_tile_set(OBJ,	HOCKEY_STICK_TILES);
	
	
	set_text_boundries( 0, VISIBLE_TILE_H-1, 0, VISIBLE_TILE_W );

	init_anims();
	init_sprites(); //hides em all
	sprite_count = 0;
	sprite_anim_count = 0;

	reset_puck( 40, 40, &sprites[sprite_count] );
	sprite_count++;

	
	for ( t=0; t<TEAM_COUNT; t++ )
	{
		for ( i=0; i<(PLAYER_COUNT/TEAM_COUNT); i++ )
		{
			//init_hockey_player( &team_players[t][i], &sprites[sprite_count], &sprites[sprite_count+1], &sprites[sprite_count+2], &sprite_animations[sprite_anim_count], t, t, DIR_S, 60 + (i * 20) , 60 + (t*20), t );
			init_hockey_player( &team_players[t][i], &sprites[sprite_count], &sprites[sprite_count+1], &sprites[sprite_count+2], &sprite_animations[sprite_anim_count], t, t, DIR_S, 0 , 0, t );
			sprite_count += 3;
			sprite_anim_count++;
		}
	}



	init_target( &targets[0] , 0, 0, TARGET_RED, &sprites[sprite_count], 0, 0);
	sprite_count++;

	init_target( &targets[1] , &puck.x, &puck.y, TARGET_YELLOW, &sprites[sprite_count], PUCK_TARGET_OFFSET_X, PUCK_TARGET_OFFSET_Y );
	sprite_count++;


	rink_unbind_camera();
	update_sprites(); //otherwise first one is screwed up
	init_rink();

	//does nothing. change the offsets instead
/*	scrollbackground( 0, 0, HUD_BG );
	scrollbackground( 0, 0, SCREEN_BG );
	scrollbackground( 0, 0, OVERHEAD_BG );
	scrollbackground( 0, 0, FLOOR_BG );
	*/

	//has no effect if bg's arent enabled yet
	clear_tile_map( BG_TILE(0,BLANK_TILES), 0);
	clear_tile_map( BG_TILE(1,BLANK_TILES), 1);
	clear_tile_map( BG_TILE(2,BLANK_TILES), 2);
	clear_tile_map( BG_TILE(3,BLANK_TILES), 3);
	rink_reset(0,0);

	//draw crowd from scratch
	if (DRAW_CROWD)
	for ( crowd_row_counter = 0; crowd_row_counter<31; crowd_row_counter++)
	{
		for(i=0;i<CROWD_COLUMN_PER_ROW_UPDATE_RATE;i++)
			update_crowd_row(crowd_row_counter,false,i);
	}



	if (DRAW_PROMO)
		draw_string(0,19,1,"Slap Stick. 2003-2017.");
	if (DRAW_SCOREBOARD)
		init_scoreboard();

	game_score[0] = 0;
	game_score[1] = 0;

	game_countdown = GAME_START_DELAY;
	start_period();

	//starts mex wave
	//MEX_WAVE_TICK(3,3) = MEX_WAVE_PEAK;
}




void goto_countdown_mode(void)
{
	//all players in place
	game_mode = MODE_COUNTDOWN;
	if ( game_countdown < GAME_RESUME_DELAY )
		game_countdown = GAME_RESUME_DELAY;
	update_custom_scoreboard_numbers( GIANT_NUMBER_ON_SCOREBOARD(game_countdown) );
	
	if (PLAYER_CONTROL_ENABLED)
		switch_local_controlled_player( &players[0] , false );
}


//dont update 0->max, otherwise it looks like a slow mem copyand you get update streaks
//need same method for delayed x/y
const crowd_row_update_order[VISIBLE_TILE_H] = {
//	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,
	17,	5,	11,	19,	15,	13,	3,	7,	0,	1,	18,	8,	4,	10,	2,	9,	6,	16,	14,	12,
};
//same as above
const crowd_column_update_order[CROWD_COLUMN_PER_ROW_UPDATE_RATE] = {
	0,	1,	2,	3,
};



void draw_debug(void)
{
	draw_text( VISIBLE_TILE_W-7,1,0,"mode%3d",game_mode);
	draw_text( VISIBLE_TILE_W-7,2,0,"cntd%3d",game_countdown);
	draw_text( VISIBLE_TILE_W-7,3,0,"ctrl%3d",players[0].control);

	draw_text( VISIBLE_TILE_W-7,5,0,"obj%4d",current_tile_data_count[0]);
	draw_text( VISIBLE_TILE_W-7,6,0,"bg0%4d",current_tile_data_count[1]);
	draw_text( VISIBLE_TILE_W-7,7,0,"bg1%4d",current_tile_data_count[2]);
	draw_text( VISIBLE_TILE_W-7,8,0,"bg2%4d",current_tile_data_count[3]);
	draw_text( VISIBLE_TILE_W-7,9,0,"bg3%4d",current_tile_data_count[4]);

	draw_text( VISIBLE_TILE_W-7,11,0,"xscr%3d",camera_x_scroll );
	draw_text( VISIBLE_TILE_W-7,12,0,"yscr%3d",camera_y_scroll );
}

void clear_debug(void)
{
	u8 l;
	//clean text off the screen
	for ( l=0; l<13; l++)
		draw_text( VISIBLE_TILE_W-7,l,0,"       ");
}



u8 game_loop(void)
{
	u8 p;

	start_frame();
	
	if (game_brightness_mod!=0)
		update_game_brightness();

	if (DRAW_CROWD)
	{
		crowd_row_counter = roll_u8(crowd_row_counter+1,0,CROWD_ROW_UPDATE_MAX*CROWD_COLUMN_PER_ROW_UPDATE_RATE);
		update_crowd_row(crowd_row_update_order[crowd_row_counter/CROWD_COLUMN_PER_ROW_UPDATE_RATE],false, crowd_column_update_order[crowd_row_counter%CROWD_COLUMN_PER_ROW_UPDATE_RATE] );
	}




//	if ( CAMERA_BIND == CAM_BIND_FREE_CAM )
//		update_free_cam();
		rink_lock_camera();




	if ( KEY_SELECT && !press_select ) //toggle debug info (DRAW_FPS)
	{
		show_debug = !show_debug;
		press_select = TRUE;
		if ( !show_debug )
			//clear debug txt
			clear_debug();
	} else if ( !KEY_SELECT && press_select )
		press_select = FALSE;

	if ( DRAW_FPS )
		draw_debug();


	
	//apply ai or input or netplay
	//updates sprite stuff too
	//SPEED ME UP
	control_players(); 

	//move the puck, check collisions etc
	update_puck(); 
	update_targets();



	switch (game_mode ) {
	//make sure players and camera are heading for the center
		case MODE_LINEUP: 
		{
			//check player's in place
			if ( update_lineup() )
			{
				goto_countdown_mode();
			}

			break;
		}
		case MODE_COUNTDOWN: 
		{

#define SPEED_SKIP_RATE 3

			if (game_countdown <=0 )
			{
				goto_play_mode();
			} else {

				game_countdown--;
				if ( (KEY_A || KEY_B) && game_countdown>SPEED_SKIP_RATE+1 ) //speed up countdown
					game_countdown -= SPEED_SKIP_RATE;

				update_custom_scoreboard_numbers( GIANT_NUMBER_ON_SCOREBOARD(game_countdown) );
			}


			break;
		}
		case MODE_PLAY: 
		{
			//check gameplay stuff, goals, out of time etc, fouls etc
			update_play();
			break;
		}
		case MODE_GOAL: //celebration
		{
			//make everyone stop, play a horn/tune etc, for a while ticks
			
			//when goal celebration is over...
			start_period();

			break;
		}

	}


//end_game_loop:
	//does my vblank etc
	if (DRAW_SCOREBOARD)
		update_scoreboard();
		//update_scoreboard( (game_countdown>0) );


	end_frame();

	return true;
}




void switch_local_controlled_player( a_hockey_player *new_control , u8 enable_control)
{
	if ( local_controlled_player )
		if ( enable_control )
			local_controlled_player->control = DEF_PLAYER_CONTROL;
	
	local_controlled_player = new_control;

	if ( local_controlled_player && PLAYER_CONTROL_ENABLED )
	{
		if ( enable_control )
			local_controlled_player->control = CONTROL_1UP;
		bind_target( &targets[0] , (u32*)&local_controlled_player->x, (u32*)&local_controlled_player->y, SPRITE_TARGET_OFFSET_X, SPRITE_TARGET_OFFSET_Y );
	} else {
		bind_target( &targets[0] , 0, 0, 0, 0 );
	}

}






void score_goal( u8 goal_number )
{
	if ( goal_number<1 || goal_number>2 )
		return;

	game_score[ goal_number-1 ]++;
	set_scoreboard_anim( SCOREBOARD_GOAL_A );

	start_period();
}





//start period
void start_period(void)
{
	u8 p;



	//sets ai target around center spot, and sets control to ai
	init_lineup();

	if ( CAMERA_BIND == CAM_BIND_PLAYER )
		rink_bind_camera( &players[0].x, &players[0].y );
	if ( CAMERA_BIND == CAM_BIND_PUCK )
		rink_bind_camera( &puck.x, &puck.y );
	if ( CAMERA_BIND == CAM_BIND_FREE_CAM )
		rink_unbind_camera();

	puck.x = (((RINK_W*TILE_W)/2)-0) << 8;
	puck.y = (((RINK_H*TILE_H)/2)-0) << 8;
	puck.x_speed = 0;
	puck.y_speed = 0;
	puck.holder = 0;

	//set target onto player_controlled_player, on a particular team

	//start period timer[when ready to be implemented]
}





//checking goals and game stuff
void update_play(void)
{
	//updating means all sprite positions are based on bound obj's pos last frame


	//check all collisions
	return;
	check_all_player_collisions();

}




#define PLAYER_ANG_AMOUNT (360/(PLAYER_COUNT))
#define LINE_UP_ANGLE_OFFSET (PLAYER_ANG_AMOUNT/2)
#define LINEUP_DIR(p) ( roll_u16( ( PLAYER_ANG_AMOUNT * (p) )+LINE_UP_ANGLE_OFFSET, 0, 360 ) )



void re_align_players(void)
{
	u8 p;
#define LINE_UP_RADIUS 50


	for (p=0; p<PLAYER_COUNT; p++)
	{
		players[p].control = CONTROL_AI_NO_BRAIN;
	//	players[p].ai_target[X] = (((RINK_W*TILE_W)/2)<<8) + (COS[ (360/PLAYER_COUNT) * p ]<<6);
	//	players[p].ai_target[Y] = (((RINK_H*TILE_H)/2)<<8) + (SIN[ (360/PLAYER_COUNT) * p ]<<6);
		players[p].ai_target[X] = (((RINK_W*TILE_W)/2)<<8) + (COS[ LINEUP_DIR(p) ] * LINE_UP_RADIUS ) - (PUCK_SPRITE_CENTER<<8);
		players[p].ai_target[Y] = (((RINK_H*TILE_H)/2)<<8) + (SIN[ LINEUP_DIR(p) ] * LINE_UP_RADIUS ) - (PUCK_SPRITE_CENTER<<8);

		
	//	players[p].ai_target[Y] = (((RINK_H*TILE_H)/2)<<8) ;
	//	players[p].ai_target[X] = p*(20<<8);

	//	players[p].x = players[p].ai_target[X];
	//	players[p].y = players[p].ai_target[Y];

	}

}



void init_lineup(void)
{
	u8 p;

	game_mode = MODE_LINEUP;

	DISABLE_LOCAL_CONTROL;
	re_align_players();
	
}




u8 update_lineup(void)
{	
	u8 players_ready = true;
	u8 p;
	u8 in_place;

#define HIT_TOLERANCE 8
	//ai controls the players for us
	for (p=0; p<PLAYER_COUNT; p++)
	{
		if (players_ready)
		{
			in_place = false;

			//if already stoppped, or if close enough. stop the player
			//if ( players[p].control == CONTROL_HALT || ( players[p].x > players[p].ai_target[X] - (8<<8) ) && ( players[p].x < players[p].ai_target[X] + (8<<8) ) ) )
			if ( ( players[p].x+PLAYER_X_OFFSET) > players[p].ai_target[X] - (HIT_TOLERANCE<<8) )
				if ( (players[p].x+PLAYER_X_OFFSET) < players[p].ai_target[X] + (HIT_TOLERANCE<<8) )
					if ( ( players[p].y+PLAYER_Y_OFFSET) > players[p].ai_target[Y] - (HIT_TOLERANCE<<8) )
						if ( (players[p].y+PLAYER_Y_OFFSET) < players[p].ai_target[Y] + (HIT_TOLERANCE<<8) )
							in_place = true;

			if (in_place)
			{
				//near enough (+-8 pixels) to position, so stop
			
				if (HALT_PLAYERS_AT_LINE_UP)
					players[p].control = CONTROL_HALT; //stops speed, as apposed to stop inputting			
				if ( LOCK_PLAYERS_AT_LINE_UP )
				{
					players[p].x = players[p].ai_target[X];
					players[p].y = players[p].ai_target[Y];
				}
				

			} else	{
				//carries on towards his ai target(near the center spot)
				players_ready = false;
			}
		}
	}

	
//	return true;
	return players_ready;
}




void control_players(void)
{
	u8 p=0;
	u8 pp;
	u32 small_dist = -1;
	u32 this_dist;
	u8 small_dist_p=0;
	
	while ( p<PLAYER_COUNT )
	{
		if ( players[p].team_no == CONTROLLED_PLAYER_TEAM )
		{
			this_dist = ( abs(puck.x - players[p].x) + abs(puck.y - players[p].y) ) / 2;
			if ( this_dist < small_dist || p==0 || p==PLAYER_COUNT )
			{
				small_dist = this_dist;
				small_dist_p = p;
			}
		}

		u8 PlayerControlled = REG_SRAM[p];
		
		switch(players[p].control)	{
		
		case CONTROL_AI:
			{
				if ( PlayerControlled == 0 )
					ai_control_player( (a_hockey_player*)&players[p] );
				break;
			}
			
		case CONTROL_AI_NO_BRAIN:
			{
				ai_control_player( (a_hockey_player*)&players[p] );
				break;
			}
		case CONTROL_1UP: 
			{
				apply_input_to_player( (a_hockey_player*)&players[p] );
				break;
			}
		case CONTROL_HALT: 
			{
				players[p].input = INPUT_NONE;
				players[p].x_speed = 0;
				players[p].y_speed = 0;
				break;
			}
		case CONTROL_OFF: 
			{
				players[p].input = INPUT_NONE;
				break;
			}
		default:  
			{
			//nothing to do
			}
		}

		if ( game_mode == MODE_PLAY )
		{
			check_and_handle_rink_collisions( &players[p] );

			if ( p< PLAYER_COUNT-1 )
				for ( pp=p+1; pp<PLAYER_COUNT; pp++)
				{
					//check player on player collisions
					check_and_handle_player_player_collisions( (a_hockey_player*)&players[pp], (a_hockey_player*)&players[p] );
				}
		}

		//updates movement/position etc based on per-player input
		update_player( &players[p] );
	
		p++;
	}

	//local player hasnt got the puck, and pressed B
//	if ( !local_controlled_player || ( local_controlled_player && (!PLAYER_GOT_PUCK(local_controlled_player)) ) && KEY_B )
	if ( ( local_controlled_player && (!PLAYER_GOT_PUCK(local_controlled_player)) ) && KEY_B )
	{
		switch_local_controlled_player( &players[small_dist_p], (game_mode==MODE_PLAY) );
	}

}




void goto_play_mode(void)
{
	u8 p;

	game_mode = MODE_PLAY;
	//should have already bound 1up controlled player to a player
	for (p=0; p<PLAYER_COUNT; p++)
	{
		players[p].control = DEF_PLAYER_CONTROL;
	}

	switch_local_controlled_player( local_controlled_player, true );

	show_custom_scoreboard(true);
}








void check_all_player_collisions(void)
{
	u8 n,i;


	for ( i=0; i<PLAYER_COUNT; i++)
	{
		if ( i< PLAYER_COUNT-1 )
			for ( n=i+1; n<PLAYER_COUNT; n++)
			{
				//check player on player collisions
				check_and_handle_player_player_collisions( (a_hockey_player*)&players[i], (a_hockey_player*)&players[n] );
			}
		check_and_handle_rink_collisions( &players[i] );
	}


}



