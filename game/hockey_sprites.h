#ifndef GAME_HOCKEY_SPRITES_H
#define GAME_HOCKEY_SPRITES_H


#include "../core/gba.h"
#include "../core/types.h"
#include "../core/sprites.h"
#include "../core/animated_sprite.h"





#define TARGET_X_OFFSET 0
#define TARGET_Y_OFFSET 0
#define TARGET_W 16
#define TARGET_H 16
#define TARGET_MAX 4

#define PLAYER_X_OFFSET 4
#define PLAYER_Y_OFFSET 8
#define PLAYER_MASK_W 8
#define PLAYER_MASK_H 8

#define PLAYER_W 16
#define PLAYER_H 16

//#define ACCEL 20
//#define DEACCEL 18 //less is more [deaccelaration] 
#define ACCEL 20
#define ACCEL_WITH_PUCK ACCEL

#define DEACCEL 17 //less is more [deaccelaration] 
//#define DEACCEL_WITH_PUCK 11 //more effective than changing accel
#define DEACCEL_WITH_PUCK 12 //more effective than changing accel

#define DEACCEL_BOUNDRY (-(DEACCEL*1)) //before we use speed-=1
#define DEACCEL_RATE (DEACCEL*1) //actual deaccel rate
#define DEACCEL_RATE_WITH_PUCK (DEACCEL_WITH_PUCK*1) //actual deaccel rate

#define LOSE_PUCK_FORCE 40


#define PLAYER_MIN_SPEED 120 //120
#define PLAYER_MAX_BOUNCE_SPEED (300)
//unused max speed
#define PLAYER_MAX_SPEED (6<<8) //never skip a tile, EVER

#define PLAYER_BURST_STRENGTH 3
#define PLAYER_NOT_PLAYER(a,b) ( (void*)a != (void*)b )
#define PLAYER_IS_PLAYER(a,b) ( (void*)a == (void*)b )

#define SHOT_ANGLE_VARY 20


#define SPRITE_TARGET_OFFSET_X 4
#define SPRITE_TARGET_OFFSET_Y 8
#define PUCK_TARGET_OFFSET_X 0
#define PUCK_TARGET_OFFSET_Y 0

#define TARGET_YELLOW 0
#define TARGET_RED 1
#define TILE_SIZE_OF_TARGET 4 //takes 4 tiles (16x16)


//pixel tolerance
#define PUCK_STICK_TOLERANCE 4


//stick/puck/player stuff
#define STICK_OFFSET_X 0
#define STICK_OFFSET_Y 4
//a is stick angle, i is stick in/out, xy is axis, sof is stick offset
#define PUCK_POS_ON_STICK_X(a,i) ( PUCK_POS_ON_STICK(a,i,X,STICK_OFFSET_X) )
#define PUCK_POS_ON_STICK_Y(a,i) ( PUCK_POS_ON_STICK(a,i,Y,STICK_OFFSET_Y) )
#define PUCK_POS_ON_STICK(a,i,xy,sof) ( (s16)((s16)stick_position[i][a][xy] + (s16)sof + (s16)puck_on_stick_position[i][a][xy] - PUCK_SPRITE_CENTER) )

//x/y same
#define PUCK_SPRITE_CENTER 3

#define STICK_SWING_RATE 5
//directions to go back on
#define STICK_SWING_PASS 2
#define STICK_SWING_SHOOT 5

#define SHOOT_MODE_NONE 0
#define SHOOT_MODE_SHOT 1
#define SHOOT_MODE_PASS 2



typedef struct {
	a_sprite *head_sprite;
	a_sprite *body_sprite;
	a_sprite *stick_sprite;
	a_sprite_anim *body_anim; //head isnt animated
	s8 direction; // 0 -> 8
	u16 bit_dir; // 0 -> 8
	u8 head_no; 
	u8 body_no;
	u8 burst_timer; //speed burst
	u8 release_timer; //countdown after dropping puck so we dont pick it up again
	u16 input; //up/dlr pass/shoot
	u32 x; //pos
	u32 y;
	u32 ai_target[2]; //aim
	s32 x_speed; //velocity
	s32 y_speed; 
	u8 team_no; //team
	u8 control; //input method
	u8 stick_angle; //updates sprite
	u8 stick_tick; //hold back for hard fire
	u8 stick_disable; //disable after hit tick max
	u8 stick_in; //or out
	u8 stick_swing_peak;
	u8 stick_shoot_mode; // SHOOT_MODE_SHOT
} a_hockey_player;


const u8 stick_position[2][8][2];

//roll dir from 0-4
//flip >= 4



typedef struct {
	u32 *bind_x;
	u32 *bind_y;
	u16 target_offset_x;
	u16 target_offset_y;
	a_sprite *sprite;
	//a_sprite_anim *anim;
	u8 target_id; //*4 
} a_object_target;




#define DIR_BIT_TO_INPUT_BIT(d) (dir_bit_to_input_bit[d])

extern const u8 stick_position[2][8][2];
extern const u8 puck_on_stick_position[2][8][2];
extern const u8 stick_layer[8];



void ai_control_player( a_hockey_player *bot );
void init_hockey_player( a_hockey_player *player, a_sprite *head_sprite, a_sprite *body_sprite, a_sprite *stick_sprite, a_sprite_anim *anim, u8 head_no, u8 body_no, s8 init_dir, u16 x, u16 y, u8 team_no);
void set_hockey_player_head( a_hockey_player *player, u8 head_no );
void set_hockey_player_body( a_hockey_player *player, u8 body_no );
void move_hockey_player( a_hockey_player *player, u32 x, u32 y);
void apply_input_to_player( a_hockey_player *player);

u8 check_and_handle_player_player_collisions( a_hockey_player *player_a, a_hockey_player *player_b);
void launch_puck( s16 perfect_angle, u8 power, u8 ang_vary);
u32 dist_from_player_to_player( a_hockey_player *player_a, a_hockey_player *player_b );

a_hockey_player *find_nearest_player( a_hockey_player *me);
a_hockey_player *find_nearest_player_on_team( a_hockey_player *me);
a_hockey_player *find_nearest_player_on_team_within_angle( a_hockey_player *me, u16 abs_angle_range);

void init_target( a_object_target *target, u32 *bind_x, u32 *bind_y, u8 target_id, a_sprite *sprite, u16 target_offset_x,  u16 target_offset_y );
void update_target( a_object_target *target );
void update_targets( void );

void update_stick_sprite( a_sprite *sprite, u8 angle, u32 player_pos_x, u32 player_pos_y, u8 stick_in );




#endif

