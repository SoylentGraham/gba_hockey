#ifndef GAME_SCOREBOARD_H
#define GAME_SCOREBOARD_H


//size in tiles
/*
#define SCOREBOARD_W 24
#define SCOREBOARD_H 4
#define SCOREBOARD_LEFT 3
#define SCOREBOARD_TOP 1
*/
#define SCOREBOARD_W 12
#define SCOREBOARD_H 3
#define SCOREBOARD_LEFT 1
#define SCOREBOARD_TOP 1


enum {
	SCOREBOARD_BLANK=0,
	SCOREBOARD_GOAL_A,
	SCOREBOARD_CUSTOM,
	SCOREBOARD_ANIM_COUNT,
};


#define GIANT_NUMBER_ON_SCOREBOARD(x)  (x/100), (x%100)





typedef struct {
	u8 frame_data[ (SCOREBOARD_W) * (SCOREBOARD_H) ];
} a_compressed_scoreboard_frame;

typedef struct {
	u8 frame_data[ (SCOREBOARD_W*2) * (SCOREBOARD_H*2) ];
} a_scoreboard_frame;


//used for casting to some anim
typedef struct {
	u8 frames;
	u8 frame_delay;
	a_scoreboard_frame *all_data;
} a_scoreboard_anim;

typedef struct {
	u8 frames;
	u8 frame_delay;
	a_compressed_scoreboard_frame *all_data;
} a_compressed_scoreboard_anim;



void update_scoreboard(void);
void draw_frame( u8 *output_frame );
void set_scoreboard_anim(u8 anim_id);





#endif

