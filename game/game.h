#ifndef GAME_GAME_H
#define GAME_GAME_H



#include "../core/gba.h"
#include "hockey_sprites.h"


//visual toggle
extern u8 show_debug;
extern u8 press_select;




//visual options
#define DRAW_CROWD 1
#define DRAW_SCOREBOARD 1		//draw the big giant scoreboard
#define DRAW_PROMO 0			// draw some promo text
#define DRAW_FPS (show_debug==TRUE)				//draw fps/stats
#define PLAYER_CONTROL_ENABLED 1	// enable 1up control
#define HALT_PLAYERS_AT_LINE_UP 1	// makes em stand still when in place, or dance around
#define LOCK_PLAYERS_AT_LINE_UP 0	// instant jump to the positions they should be at
#define DEF_PLAYER_CONTROL CONTROL_AI	// AI, or OFF/1UP to test

//#define CAMERA_BIND CAM_BIND_FREE_CAM	// puck or player (free cam aint finished)
#define CAMERA_BIND CAM_BIND_PUCK	
//#define CAMERA_BIND CAM_BIND_PLAYER

#define AUTO_SWITCH_CONTROL	1
#define CONTROLLED_PLAYER_TEAM 0
#define STOP_DIRECTIONAL_MOVEMENT_WHEN_SWINGING 1
//8
#define PLAYER_COUNT 8 //8
#define TEAM_COUNT 2
//each player costs 2 sprites

#define HUD_BG 0	//unused for sprites
#define SCREEN_BG 1 //transparent over players/goals rink
#define OVERHEAD_BG 2	//apears above body
#define HEAD_BG 2	//apears above body
#define BODY_BG 3	//semi transparent
#define FLOOR_BG 3

/*//in types.h
#define OBJ 0
#define BG(n) (1+(n>2?2:n))
*/

//#define RAND(n) (REG_VCOUNT%n)

//get player number from pointer compared to root
//p is pointer to a_player
#define PLAYER_NUMBER(p) ( ((void*)p - (void*)players) / sizeof(a_hockey_player) )




void init_game(void);
u8 game_loop(void);
void get_xy_speed_from_ang(s16 ang, s16 speed_mod, s32 *x_speed, s32 *y_speed);
void score_goal( u8 goal_number );

void re_align_players(void);
void start_period(void);
void init_lineup(void);
void check_all_player_collisions(void);

void update_play(void);
void control_players(void);
void goto_play_mode(void);

#define DISABLE_LOCAL_CONTROL switch_local_controlled_player(0,false)

void switch_local_controlled_player( a_hockey_player *new_control , u8 enable_control);



#endif

