#ifndef GAME_MAIN_H
#define GAME_MAIN_H



#include "../core/gba.h"
#include "hockey_sprites.h"



//staringmonkeys timer defines
#define REG_TM0D       *(volatile u16*)0x4000100		//Timer 1 Data
#define REG_TM0CNT     *(volatile u16*)0x4000102		//Timer 1 Control
#define REG_TM1D       *(volatile u16*)0x4000104		//Timer 2 Data
#define REG_TM1CNT     *(volatile u16*)0x4000106		//Timer 2 Control
#define REG_TM2D       *(volatile u16*)0x4000108		//Timer 3 Data
#define REG_TM2CNT     *(volatile u16*)0x400010A		//Timer 3 Control
#define REG_TM3D       *(volatile u16*)0x400010C		//Timer 4 Data
#define REG_TM3CNT     *(volatile u16*)0x400010E		//Timer 4 Control
/////////////////Bits////////////////
#define BIT00 1
#define BIT01 2
#define BIT02 4
#define BIT03 8
#define BIT04 16
#define BIT05 32
#define BIT06 64
#define BIT07 128
#define BIT08 256
#define BIT09 512
#define BIT10 1024
#define BIT11 2048
#define BIT12 4096
#define BIT13 8192
#define BIT14 16384
#define BIT15 32768
///////////////////Definitions////////////////////
#define FREQUENCY_0		0x00
#define FREQUENCY_64	BIT00
#define FREQUENCY_256	BIT01
#define FREQUENCY_1024	BIT00 | BIT01

#define TIMER_CASCADE	BIT02
#define TIMER_IRQ		BIT06
#define TIMER_ENABLE	BIT07


#define REG_INTERUPT   *(volatile u32*)0x3007FFC		//Interrupt Register



//ordred dirs
enum {
	DIR_N=0,
	DIR_NE,
	DIR_E,
	DIR_SE,
	DIR_S,
	DIR_SW,
	DIR_W,
	DIR_NW,
	DIR_COUNT, // max dirs
};

//bit relative dirs
enum {
	NO_DIR=0,
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST,
	//DIR_COUNT,
};



//#define DIR_BIT_N BIT(1)
//#define DIR_BIT_S BIT(2)
//#define DIR_BIT_E BIT(3)
//#define DIR_BIT_W BIT(4)
#define DIR_BIT_N 1
#define DIR_BIT_S 2
#define DIR_BIT_E 4
#define DIR_BIT_W 8
#define DIR_BIT_GOAL 16

#define DIR_BIT_NE ( DIR_BIT_N | DIR_BIT_E )
#define DIR_BIT_NW ( DIR_BIT_N | DIR_BIT_W )
#define DIR_BIT_SE ( DIR_BIT_S | DIR_BIT_E )
#define DIR_BIT_SW ( DIR_BIT_S | DIR_BIT_W )



#define INPUT_NONE BIT(0)
#define INPUT_UP BIT(1)
#define INPUT_DOWN BIT(2)
#define INPUT_LEFT BIT(3)
#define INPUT_RIGHT BIT(4)
#define INPUT_PASS BIT(5)
#define INPUT_SHOOT BIT(6)

enum{
	CONTROL_HALT=0, //stops as fast as possible
	CONTROL_OFF, //no more input
	CONTROL_AI, //ai controls
	CONTROL_AI_NO_BRAIN, //ai moves, but doesnt change target
	CONTROL_1UP, //use player to input
	CONTROL_NET, //future expansion =D
};

enum{
	CAM_BIND_FREE_CAM=0,
	CAM_BIND_PUCK,
	CAM_BIND_PLAYER,
};

enum{
	MODE_INIT=0,
	MODE_LINEUP,
	MODE_COUNTDOWN,
	MODE_PLAY,
	MODE_GOAL,
};
extern u8 game_mode; //^^ above mode


//fps * timedelay = GAME_RESUME_DELAY 
#define GAME_RESUME_DELAY 222
#define GAME_START_DELAY 444
//#define GAME_RESUME_DELAY 123
//#define GAME_START_DELAY 333

#define GAME_IN_PLAY ( game_mode == MODE_PLAY ) 

#define HOCKEY_TEAM_COUNT 2
#define HOCKEY_PLAYER_COUNT 4

#define DIR_TO_DIR_BIT(d) (dir_to_dir_bit[d])

#define DIR_BIT_TO_DIR(d) (dir_bit_to_dir(d))
#define ANG_GAP (360/DIR_COUNT)
#define DIR_ANG(dir) ( ANG_GAP*dir )




#define TEMP(n) (n)
#define GENERIC_MASK ((u8*)generic_mask_data)
#define RAND(n) (rand()%n)
#define OPPOSITE_ANGLE(n) ( roll_u8(n+(DIR_COUNT/2), 0, DIR_COUNT) )


const s8 dir_data[DIR_COUNT*2];
const s8 dir_data_x[DIR_COUNT];
const s8 dir_data_y[DIR_COUNT];

#define FORCE_RATE 10 //bigger == smaller force
#define FORCE_OF_SPEED(xspd,yspd,dir) ( (abs(xspd)+abs(yspd)) / ( ( abs(dir_data_x[dir])+abs(dir_data_y[dir]) )*FORCE_RATE ) )

const u8 A_TAN[16320];

#define DISTANCE_FROM_COORD(x1,y1,x2,y2) ( sqrt_u32 ( SQUARED( (x2) - (x1) ) + SQUARED( (y2) - (y1) ) ) )
#define SQUARED(n) ( (n)*(n) )




u32 sqrt_u32(u32 n);
u16 angle_to_player_from_player( a_hockey_player *player_a, a_hockey_player *player_b );
u16 get_angle_from_gradients(s16 x_gradient, s16 y_gradient);
void get_xy_speed_from_ang(s16 ang, s16 speed_mod, s32 *x_speed, s32 *y_speed);
void modify_xy_speed_on_collision( u8 collision, s32 *x_speed, s32 *y_speed, s32 speed_mod, s32 min_speed, s32 max_speed);
s32 bigger(s32 a, s32 b);
u8 dir_bit_to_dir( u8 dir_bit );
s16 difference_between_angles( u16 ang_a, u16 ang_b );
u16 angle_to_coord( u32 x1, u32 y1, u32 x2, u32 y2);

void init_interrupt(void);
void IRQHandler(void);



void start_frame(void);
void end_frame(void);




#endif
