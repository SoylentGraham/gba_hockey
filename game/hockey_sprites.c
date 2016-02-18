#include "hockey_sprites.h"
#include "rink.h"
#include "rink_map.h"
#include "puck.h"
#include "main.h"
#include "game.h"

#include "../core/gba.h"
#include "../core/core.h"
#include "../core/types.h"
#include "../core/collisions.h"
#include "../core/tile_alloc.h"
#include "../core/math.h"
#include "../core/sprites.h"
#include "../core/animated_sprite.h"
#include "../core/keypad.h"
#include "../media/gfx/media.h"



extern a_hockey_player team_players[TEAM_COUNT][PLAYER_COUNT/TEAM_COUNT];
extern const a_hockey_player *players;
extern a_puck puck;

extern s16 camera_tiny_x_scroll;
extern s16 camera_tiny_y_scroll;
extern s16 camera_x_scroll;
extern s16 camera_y_scroll;
a_object_target targets[TARGET_MAX];


const u8 dir_to_dir_bit[DIR_COUNT] ={ 
	DIR_BIT_N, DIR_BIT_NE, DIR_BIT_E, DIR_BIT_SE, DIR_BIT_S, DIR_BIT_SW, DIR_BIT_W, DIR_BIT_NW 
};
const u8 dir_bit_to_input_bit[DIR_COUNT] ={ 
	INPUT_UP, INPUT_UP|INPUT_RIGHT, INPUT_RIGHT, INPUT_DOWN|INPUT_RIGHT, INPUT_DOWN, INPUT_DOWN|INPUT_LEFT, INPUT_LEFT, INPUT_UP|INPUT_LEFT
};




//pos of stick top left when being held
// pos[in][dir][xy]
const u8 stick_position[2][8][2] = {
//position (out)
	5,	0,
	8,	0,
	8,	5,
	8,	8,
	4,	8,
	0,	7,
	0,	6,
	0,	0,
//position(in)
	5,	2,	
	6,	2,
	6,	5,
	6,	6,
	3,	6,
	2,	6,
	2,	6,
	2,	2,
};

//pos of puck on the stick
const u8 puck_on_stick_position[2][8][2] = {
//position (out)
	2,	0,
	6,	1,
	7,	2,
	6,	6,
	4,	7,
	1,	6,
	0,	5,
	1,	1,
//position(in)
	2,	0,
	6,	1,
	7,	2,
	6,	6,
	4,	7,
	1,	6,
	0,	5,
	1,	1,
};


const u8 stick_layer[8] = {
//HEAD_BG, OVER_BG, BODY_BG
	BODY_BG,
	BODY_BG,
	OVERHEAD_BG,
	OVERHEAD_BG,
	OVERHEAD_BG,
	OVERHEAD_BG,
	OVERHEAD_BG,
	BODY_BG,
};



void apply_input_to_player( a_hockey_player *player)
{
	if (!player) return;

	player->input = INPUT_NONE;

	if ( !STOP_DIRECTIONAL_MOVEMENT_WHEN_SWINGING || ( STOP_DIRECTIONAL_MOVEMENT_WHEN_SWINGING && !player->stick_disable ) )
	{
		if (KEY_UP)
			player->input |= INPUT_UP;

		if (KEY_DOWN)
			player->input |= INPUT_DOWN;

		if (KEY_LEFT)
			player->input |= INPUT_LEFT;

		if (KEY_RIGHT)
			player->input |= INPUT_RIGHT;
	}

	if (KEY_A)
		player->input |= INPUT_SHOOT; //burst of speed

	if (KEY_B)
		player->input |= INPUT_PASS; //and tackle
}




void init_hockey_player( a_hockey_player *player, a_sprite *head_sprite, a_sprite *body_sprite, a_sprite *stick_sprite, a_sprite_anim *anim, 
						u8 head_no, u8 body_no, s8 init_dir, u16 x, u16 y, u8 team_no)
{
	player->body_anim = anim;
	player->body_sprite = body_sprite;
	player->head_sprite = head_sprite;
	player->stick_sprite = stick_sprite;

	player->head_no = head_no;
	player->body_no = body_no;
	player->direction = init_dir;
	player->input = INPUT_NONE;
	player->x_speed = 0;
	player->y_speed = 0;
	player->x = x<<8;
	player->y = y<<8;
	player->burst_timer = 0;
	player->release_timer = 0;
	player->team_no = team_no;

	player->stick_angle = DIR_S;
	player->stick_in = 0;
	player->stick_tick = 0; //same ang as player

//	init_sprite( player->stick_sprite, SIZE_8, SQUARE, 0, 0, HEAD_BG, 0, 0, 0 );
//	move_sprite( player->stick_sprite, 0, 0 );

//	make_sprite_anim( player->body_anim, player->body_sprite, HOCKEY_GUY_BODY_TILES, 0/*dir*/, 1, 4, 1, 100);

	init_sprite( player->body_sprite, SIZE_8, WIDE, HOCKEY_GUY_BODY_TILES, (body_no*2)*2, BODY_BG, 0, false, false);
	init_sprite( player->head_sprite, SIZE_8, SQUARE, HOCKEY_GUY_HEAD_TILES, head_no*2, HEAD_BG, 0, false, false );
	set_hockey_player_head( player, init_dir );
	set_hockey_player_body( player, init_dir );

}



void set_hockey_player_head( a_hockey_player *player, u8 head_no )
{
	u8 display_head;
	u16 mirror = true;
	display_head = roll_u8 ( head_no, DIR_N, DIR_SW);
	
	if (head_no>4)
	{
		display_head = 8 - head_no;
		//mirror = false;
	}

	mirror = ( player->bit_dir & DIR_BIT_E );

	display_head += player->head_no * DIR_SW;

	//last param decides on flip
	init_sprite( player->head_sprite, SIZE_8, SQUARE, HOCKEY_GUY_HEAD_TILES, display_head*2, HEAD_BG, 0, false, mirror );
//	set_sprite_tile ( player->head_sprite, OBJ_TILE( HOCKEY_GUY_HEAD_TILES, (display_head*2) ) );
//	mirror_sprite( player->head_sprite, mirror );

	move_hockey_player( player, player->x, player->y);
}


void set_hockey_player_body( a_hockey_player *player, u8 body_no )
{
	u8 display_body;
	u16 mirror = true;
	display_body = roll_u8 ( body_no, DIR_N, DIR_SW);
	
	if (body_no>4)
	{
		display_body = 8 - body_no;
		//mirror = false;
	}

	mirror = ( player->bit_dir & DIR_BIT_E );
	display_body += player->body_no * DIR_SW;

	set_sprite_tile ( player->body_sprite, OBJ_TILE( HOCKEY_GUY_BODY_TILES, (display_body*2)*2 ) );
	mirror_sprite( player->body_sprite, mirror );

	make_sprite_anim( player->body_anim, player->body_sprite, HOCKEY_GUY_BODY_TILES, (display_body*4)/*dir*/, 2, 4, 1, 8);
	//make_sprite_anim( player->body_anim, player->body_sprite, HOCKEY_GUY_BODY_TILES, (display_body*1), 2, 1, 1, 8);

//	move_hockey_player( player, player->x, player->y);
}






//top left
void move_hockey_player( a_hockey_player *player, u32 x, u32 y)
{
	s32 xx,yy;
	u8 hide = 1;

	xx = x;
	yy = y;
//	xx -= (RINK_CAMERA_X<<8) ;
//	yy -= (RINK_CAMERA_Y<<8) ;

	hide = coord_visible(xx, yy, 20, 20);

	if (hide==1)
	{
		//xx -= (RINK_CAMERA_X<<8) ;
		//yy -= (RINK_CAMERA_Y<<8) ;
		move_sprite( player->head_sprite, ( ( xx ) >>8) + 4 - RINK_CAMERA_X, ( ( yy ) >>8) + 0 - RINK_CAMERA_Y );
		move_sprite( player->body_sprite, ( ( xx ) >>8) + 0 - RINK_CAMERA_X, ( ( yy ) >>8) + 8 - RINK_CAMERA_Y );
	} else {
		hide_sprite( player->head_sprite );
		hide_sprite( player->body_sprite );
	}

}


void update_player( a_hockey_player *player)
{
	u8 old_dir = player->direction;
	s32 burst[2]; //x y burst of speed
	u8 accel_rate,de_accel_rate;
	u8 swing_max;
	u32 goal_at[2];
	//one variable, many uses
	a_hockey_player *alt_player;
	u16 ang_to_goal;
	s16 diff_to_goal;
	
	
	//update inputs
	player->bit_dir = 0;


	if ( player->input & INPUT_UP )
		player->bit_dir |= DIR_BIT_N;

	if ( player->input & INPUT_DOWN )
		player->bit_dir |= DIR_BIT_S;

	if ( player->input & INPUT_LEFT )
		player->bit_dir |= DIR_BIT_W;

	if ( player->input & INPUT_RIGHT )
		player->bit_dir |= DIR_BIT_E;


	player->direction = DIR_BIT_TO_DIR( player->bit_dir );
	if (player->direction == DIR_COUNT)
		player->direction = old_dir;
	player->bit_dir  = DIR_TO_DIR_BIT( player->direction );

	//cant pick up puck if we just fired it
	if (player->release_timer > 0)
		player->release_timer--;
	//control bursts of speed
	if ( player->burst_timer > 0 )
		player->burst_timer--;
	
	if ( player->stick_tick > 0 )
		player->stick_tick --;
	

	if ( player->input & INPUT_SHOOT )
		if (PLAYER_GOT_PUCK(player))
		{
			if ( player->stick_disable == false )
				player->stick_tick += 2;
			//if ( player->stick_shoot_mode == SHOOT_MODE_NONE )
				player->stick_shoot_mode = SHOOT_MODE_SHOT;

		} else {
			//quikc burst of speed
			if ( player->burst_timer == 0 && player->release_timer==0 )
			{
				get_xy_speed_from_ang( DIR_ANG(player->direction), PLAYER_BURST_STRENGTH, &burst[X], &burst[Y]);

				player->x_speed += burst[X];
				player->y_speed += burst[Y];
				player->burst_timer = 20; //x ticks before i can burst some speed again
			}
		}
	
	


	if ( player->input & INPUT_PASS )
		if (PLAYER_GOT_PUCK(player))
		{
			if ( player->stick_disable == false )
				player->stick_tick += 2;
			//if ( player->stick_shoot_mode == SHOOT_MODE_NONE )
				player->stick_shoot_mode = SHOOT_MODE_PASS;

		} else {
			//tackle

		}


		
	//hit max swing, stop player from swinging back
	if ( player->stick_shoot_mode == SHOOT_MODE_PASS )
		swing_max = STICK_SWING_PASS; 
	else 
		swing_max = STICK_SWING_SHOOT; 

	if (player->stick_tick >= swing_max * STICK_SWING_RATE )
	{
		player->stick_disable = true;
		player->stick_swing_peak = player->stick_tick;
		if ( player->stick_swing_peak/STICK_SWING_RATE <= 0 )
			player->stick_swing_peak = STICK_SWING_RATE * 1;

	}

	//got the puck, stick was held back, not being held anymore
	if (PLAYER_GOT_PUCK(player) && player->stick_tick>0 && !player->stick_disable )
		if ( !(player->input & INPUT_PASS) && !(player->input & INPUT_SHOOT) )
			if ( player->stick_shoot_mode != SHOOT_MODE_NONE )
			{
				//will start to fire
				player->stick_disable = true;
				player->stick_swing_peak = player->stick_tick;
				//player->stick_shoot_mode = SHOOT_MODE_NONE;
			}



	/*** handle shooting/passing ***/

	//reached bottom end of the swing, crack!
	if ( player->stick_tick <= STICK_SWING_RATE && player->stick_disable )
	{
		player->stick_disable = false;
		//shoot 
		if ( player->stick_shoot_mode == SHOOT_MODE_SHOT )
		{
			goal_at[X] = REAL_GOAL_CENTER_X( (player->team_no==1) ); //if !1 then 0 (!=1)
			goal_at[Y] = REAL_GOAL_CENTER_Y( (player->team_no==1) ); //if !1 then 0 (!=1)

			ang_to_goal = DIR_ANG( player -> direction );
			ang_to_goal = angle_to_coord( player->x, player->y, goal_at[X], goal_at[Y]);
			diff_to_goal = difference_between_angles( DIR_ANG( player -> direction ), ang_to_goal );
		
			if ( abs(diff_to_goal) < 135 )
				launch_puck( ang_to_goal, (player->stick_swing_peak/STICK_SWING_RATE) , 10 );
		}

		 //pass
		if ( player->stick_shoot_mode == SHOOT_MODE_PASS )
		{
			alt_player = find_nearest_player_on_team(player);
			if ( alt_player )
				launch_puck( angle_to_player_from_player( player, alt_player ), PUCK_PASS_SPEED, 0 );
		}
	}
	



/*
	draw_text(0,10,true,"%3d",player->bit_dir);
	draw_text(0,11,true,"%3d", ( player->bit_dir & DIR_BIT_E ) );
	draw_text(0,12,true,"%3d", player->direction );
*/		
	//doing only when direction changes keeps animation going
	if ( player->direction != old_dir )
	{
		set_hockey_player_head( player, player->direction );
		set_hockey_player_body( player, player->direction );
	}
	player->stick_angle = roll_u8(player->direction + (player->stick_tick/STICK_SWING_RATE), 0, DIR_COUNT );
	move_hockey_player( player, player->x, player->y);
	update_stick_sprite( player->stick_sprite, player->stick_angle, player->x , player->y , player->stick_in );


/*	if ( player->input & INPUT_LEFT )
		player->x_speed -= (( ACCEL * COS [ DIR_ANG( player->direction ) ] )>>8);
	if ( player->input & INPUT_RIGHT )
		player->x_speed += (( ACCEL * COS [ DIR_ANG( player->direction ) ] )>>8);

	if ( player->input & INPUT_UP )
		player->y_speed -= (( ACCEL * SIN [ DIR_ANG( player->direction ) ] )>>8);
	if ( player->input & INPUT_DOWN )
		player->y_speed += (( ACCEL * SIN [ DIR_ANG( player->direction ) ] )>>8);
*/

	//36
	//17
	//*1

	if ( PLAYER_GOT_PUCK( player ) )
	{
		accel_rate = ACCEL_WITH_PUCK; 
		de_accel_rate = DEACCEL_RATE_WITH_PUCK;
	} else {
		accel_rate = ACCEL; 
		de_accel_rate = DEACCEL_RATE;
	}

	
	//disable move input if used a burst of speed
	if ( player->burst_timer < 10 )
	{
		if ( player->input & INPUT_LEFT )
			player->x_speed -= accel_rate;
		if ( player->input & INPUT_RIGHT )
			player->x_speed += accel_rate;

		if ( player->input & INPUT_UP )
			player->y_speed -= accel_rate;
		if ( player->input & INPUT_DOWN )
			player->y_speed += accel_rate;
	}




	//slowdown
	if ( player->x_speed < -PLAYER_MIN_SPEED )
	{
		if ( player->x_speed > -DEACCEL_BOUNDRY )
			player->x_speed += 1;
		else
			player->x_speed += -player->x_speed/ de_accel_rate ;

	} else 	if ( player->x_speed > PLAYER_MIN_SPEED ) 
	{
		if ( player->x_speed < DEACCEL_BOUNDRY )
			player->x_speed -= 1;
		else
			player->x_speed -= player->x_speed/ de_accel_rate ;
	}


	if ( player->y_speed<-PLAYER_MIN_SPEED )
	{
		if ( player->y_speed > -DEACCEL_BOUNDRY )
			player->y_speed += 1;
		else
			player->y_speed += -player->y_speed / de_accel_rate ;
	} else 	if ( player->y_speed>PLAYER_MIN_SPEED )  {
		if ( player->y_speed < DEACCEL_BOUNDRY )
			player->y_speed -= 1;
		else
			player->y_speed -= player->y_speed/ de_accel_rate ;
	}


	player->x += player->x_speed ;
	player->y += player->y_speed ;

//	draw_text(12,15,true,"x%5d y%5d" , player->x_speed, player->y_speed );
	
}






//1 or 0
u8 check_and_handle_player_player_collisions( a_hockey_player *player_a, a_hockey_player *player_b)
{
	u8 crash_id[2];
	s16 p[2][2];
	u8 i;
	s32 force[2];
	s32 tmp_f;

	u32 dist=0;
	
	//dist = dist_from_player_to_player( player_a, player_b );
	//if (dist>40) return 0;


	p[0][X] = (player_a->x>>8) + 4;
	p[0][Y] = (player_a->y>>8) + 0;

	p[1][X] = (player_b->x>>8) + 4;
	p[1][Y] = (player_b->y>>8) + 0;





	if ( check_collisions ( 8, 8, 8, 8,	GENERIC_MASK, GENERIC_MASK, p[0][X] , p[0][Y], p[1][X], p[1][Y], &crash_id[0], &crash_id[1], generic_mask_u8 , generic_mask_u8 ) )
	//if ( check_collisions ( 8, 8, 8, 8,	GENERIC_MASK, GENERIC_MASK, p[0][X] , p[0][Y], p[1][X], p[1][Y], &crash_id[0], &crash_id[1], 0 , 0 ) )
	{
		//use other way around collision sides
		force[0] = FORCE_OF_SPEED(player_a->x_speed,player_a->y_speed,player_a->direction);
		force[1] = FORCE_OF_SPEED(player_b->x_speed,player_b->y_speed,player_b->direction);

		tmp_f = force[0];

		force[0] += force[1] / 2;
		force[1] += tmp_f / 2;




		if ( PLAYER_GOT_PUCK(player_a) )
		{
			//draw_text(0,2,0,"%4d",force[0]);

			if ( force[1] > LOSE_PUCK_FORCE )
			{
				player_b -> release_timer = 20; //stop other player instantly catching it
				launch_puck( DIR_ANG( OPPOSITE_ANGLE( player_a -> direction + 2 ) ), PUCK_LOSE_SPEED, 45  );
			}
		}

		if ( PLAYER_GOT_PUCK(player_b) )
		{
		//	draw_text(0,2,0,"%4d",force[1]);
			if ( force[0] > LOSE_PUCK_FORCE )
			{
				player_a -> release_timer = 20; //other player cant get it either
				launch_puck( DIR_ANG( OPPOSITE_ANGLE( player_b -> direction + 2 ) ), PUCK_LOSE_SPEED, 45 );
			}
		}

		//speed mod is the average "force" of the other player
		modify_xy_speed_on_collision( crash_id[0], &player_a->x_speed, &player_a->y_speed, force[1]*2  , PLAYER_MIN_SPEED, PLAYER_MAX_BOUNCE_SPEED );
		modify_xy_speed_on_collision( crash_id[1], &player_b->x_speed, &player_b->y_speed, force[0]*2  , PLAYER_MIN_SPEED, PLAYER_MAX_BOUNCE_SPEED );

		return true;
	}

	return false;
}





void launch_puck( s16 perfect_angle, u8 power, u8 ang_vary) //we add on our own variation
{
	//const u16 
	s16 ang_ch = RAND(ang_vary ) - (ang_vary /2);

	if ( !puck.holder ) return;

	//output_ang = roll_s16( output_ang, 0, 360 );

	get_xy_speed_from_ang( perfect_angle  + ang_ch , power, &puck.x_speed, &puck.y_speed);

	//puck.release = 10; //ticks before puck can be picked up again
	puck.holder->release_timer = 20;
	puck.holder->burst_timer = 20; //stop em bursting too
	//release from player
	puck.holder = 0;
}


a_hockey_player *find_nearest_player( a_hockey_player *me)
{
	u8 i;
	u32 dist,little_dist;
	s16 found = -1;

	if (!me) return 0;

	for (i=0; i<PLAYER_COUNT; i++)
	{
		if ( PLAYER_NOT_PLAYER(me, &players[i]) )
		{
			dist = dist_from_player_to_player( me, (a_hockey_player*)&players[i] );
			if ( found==-1 || little_dist>dist )
			{
				little_dist = dist;
				found = i;
			}
		}
	}

	if (found==-1)	return 0;
	return ((a_hockey_player*) &players[found]);
}



a_hockey_player *find_nearest_player_on_team( a_hockey_player *me)
{
	u8 i;
	u32 dist,little_dist;
	s16 found = -1;

	if (!me) return 0;

	for (i=0; i<PLAYER_COUNT/TEAM_COUNT; i++)
	{
		if ( PLAYER_NOT_PLAYER(me, &team_players[me->team_no][i]) )
		{
			dist = dist_from_player_to_player( me, &team_players[me->team_no][i] );
			if ( found==-1 || little_dist>dist )
			{
				little_dist = dist;
				found = i;
			}
		}
	}

	if (found==-1)	return 0;
	return &team_players[me->team_no][found];
}



a_hockey_player *find_nearest_player_on_team_within_angle( a_hockey_player *me, u16 abs_angle_range)
{
	u8 i;
	u32 dist,little_dist;
	s16 found = -1;
	s16 ang_diff;
	u16 ang_to;
	u16 my_ang;

	//abs_angle_range

	if (!me) return 0;

	my_ang = DIR_ANG( me-> direction );

	for (i=0; i<PLAYER_COUNT/TEAM_COUNT; i++)
	{
		if ( PLAYER_NOT_PLAYER(me, &team_players[me->team_no][i]) )
		{
			dist = dist_from_player_to_player( me, &team_players[me->team_no][i] );
			if ( found==-1 || little_dist>dist )
			{
				ang_to = angle_to_player_from_player( me, &team_players[me->team_no][i] );
				ang_diff = difference_between_angles ( my_ang, ang_to );
				if ( abs(ang_diff) <= abs_angle_range )
				{
					little_dist = dist;
					found = i;
				}
			}
		}
	}

	if (found==-1)	return 0;
	return &team_players[me->team_no][found];
}




u32 dist_from_player_to_player( a_hockey_player *player_a, a_hockey_player *player_b )
{
	return DISTANCE_FROM_COORD( (player_b->x >>8), (player_b->y >>8), (player_a->x >>8), (player_a->y >>8) );
}




void ai_update_target( a_hockey_player *bot )
{
	bot->ai_target[X] = puck.x;
	bot->ai_target[Y] = puck.y;

	if ( PLAYER_IS_PLAYER( puck.holder, bot ) )
	{
		//set to goal
		bot->ai_target[X] = REAL_GOAL_CENTER_X(bot->team_no);
		bot->ai_target[Y] = REAL_GOAL_CENTER_Y(bot->team_no);
	}

}

//update what we're aiming for
//and then direct our input towards it
void ai_control_player( a_hockey_player *bot )
{
	u32 head_angle; //0->360
	u16 head_dir; //0->16
	u32 head_dist;
	u32 n,m;


	bot->input = INPUT_NONE;

	//stop players from moving if teammate has puck
	if ( puck.holder && puck.holder->team_no == bot->team_no && bot!=puck.holder)
		return;

	if ( bot->control != CONTROL_AI_NO_BRAIN )
		ai_update_target( bot );

//	aim_for[X] = puck.x >> 8;
//	aim_for[Y] = puck.y >> 8;
//	head_dist = DISTANCE_FROM_COORD( (bot->x>>8), (bot->y>>8), aim_for[X], aim_for[Y] );

/*	if ( PLAYER_NOT_PLAYER( bot, puck.holder )  )
	{
		aim_for[X] = puck.x ;
		aim_for[Y] = puck.y ;
	} else {
		//center of rink
		aim_for[X] = ( (RINK_W*TILE_W)/2 ) <<8 ;
		aim_for[Y] = ( (RINK_H*TILE_H)/2 ) <<8 ;
	}
*/

//	if ( puck.holder )
//		if ( puck.holder->team_no == bot->team_no )
//			if ( PLAYER_NOT_PLAYER( puck.holder, bot ) )
//				return;


	/*
	head_dist = DISTANCE_FROM_COORD( bot->x, bot->y, bot->ai_target[X], bot->ai_target[Y] );
	if ( head_dist < (6<<8) )		return;
*/


	n = abs(bot->x - bot->ai_target[X]);
	if ( n < 3<<8 ) return;
	
	m = abs(bot->y - bot->ai_target[Y]);
	if ( m < 3<<8 ) return;

	n = n+m+((n+m)/2) ;
	if ( n < 6<<8 ) return;


	head_angle = angle_to_coord( bot->x, bot->y, bot->ai_target[X], bot->ai_target[Y] );
	head_dir = head_angle/ANG_GAP;


	bot->input = DIR_BIT_TO_INPUT_BIT( head_dir );

}



void bind_target( a_object_target *target, u32 *bind_x, u32 *bind_y, u16 target_offset_x,  u16 target_offset_y )
{
	if (!target) return;
	target->bind_x = bind_x;
	target->bind_y = bind_y;
	target->target_offset_x = target_offset_x;
	target->target_offset_y = target_offset_y;
}



u8 init_target_sprite( a_object_target *target )
{
	//turn into a target if visible, or arrow if not
	if ( coord_visible( *target->bind_x, *target->bind_y, 16,16) )
	{
		init_sprite( target->sprite, SIZE_16, SQUARE, TARGET_TILES, ( target->target_id * TILE_SIZE_OF_TARGET )*2, FLOOR_BG, 0, false, false );
		return 1; //save calc;ing visible again
	} else 	{
		init_sprite( target->sprite, SIZE_8, SQUARE, ARROW_TILES, DIR_N*2, FLOOR_BG, 0, false, false );
		return 0;
	}
}



//turns into a target
void init_target( a_object_target *target, u32 *bind_x, u32 *bind_y, u8 target_id, a_sprite *sprite, u16 target_offset_x,  u16 target_offset_y )
{
	if (!target) return;
	if (!sprite) return;

	bind_target( target, bind_x, bind_y, target_offset_x, target_offset_y );

	target->sprite = sprite;

	target->target_id = target_id;

	//done below
//	init_target_sprite(target);

	update_target(target);

}




void update_target( a_object_target *target )
{
	s32 xx,yy;
	u8 bind_is_visible;
	u16 ang;

	if ( !target )	return;
	if ( !target->sprite )	return;
	
	if ( !target->bind_x || !target->bind_y )
	{
		hide_sprite( target->sprite );
		return;
	}

	xx = (*target->bind_x);
	yy = (*target->bind_y);

	bind_is_visible = init_target_sprite( target );

	if ( bind_is_visible )
	{
		xx = ( ( xx ) >>8) - RINK_CAMERA_X + target->target_offset_x ;
		yy = ( ( yy ) >>8) - RINK_CAMERA_Y + target->target_offset_y ;
//		move_sprite( target->sprite, ( ( xx ) >>8) - RINK_CAMERA_X + target->target_offset_x, ( ( yy ) >>8) - RINK_CAMERA_Y + target->target_offset_y );

	} else {
		//calc direction towards player thats offscreen
		//angle from center of screen
		ang = angle_to_coord( (RINK_CAMERA_X+(240/2))<<8, (RINK_CAMERA_Y+(160/2))<<8, xx, yy );

		//ang = roll_u16( ang+(45/2), 0, 360 );

		//change the sprite to this angle
		set_sprite_tile( target->sprite, OBJ_TILE(ARROW_TILES,  ang/ANG_GAP ) *2 );

		// RINK_CAMERA_X
		// RINK_CAMERA_X+(320)
		// RINK_CAMERA_Y
		// RINK_CAMERA_Y+(240)

		//camera is top left!

		xx = xx>>8;
		yy = yy>>8;

		yy -= RINK_CAMERA_Y;
		xx -= RINK_CAMERA_X;

	
		//keep arrow in screen
		if (xx< 10 )
			xx = 10 ;

		if (yy< 10 )
			yy = 10;

		if (xx>240-8 -10)
			xx = 240 - 8 -10; //arrow_w

		if (yy>160-8 -10 )
			yy = 160 - 8 -10; // arrow_h
	

		xx += 4;
		yy += 4;

		//hide_sprite( target->sprite );
	}


	move_sprite( target->sprite, xx, yy );
}


void update_targets( void )
{
	u8 t;
	for (t=0; t<TARGET_MAX; t++)
		update_target(&targets[t]);
}





void update_stick_sprite( a_sprite *sprite, u8 angle, u32 player_pos_x, u32 player_pos_y, u8 stick_in )
{
	u8 ang,hide,flip_mirror;
	s16 x,y;

	if (!sprite) return;

	angle = roll_u8( angle, 0, 8 );
	ang = roll_u8( angle, 0, 4 );
	
	x = (player_pos_x>>8) ;
	y = (player_pos_y>>8) ;

	x += stick_position[stick_in][angle][X];
	y += stick_position[stick_in][angle][Y];

	//pixel offset
	x += STICK_OFFSET_X ;
	y += STICK_OFFSET_Y ;

	hide = coord_visible(x<<8, y<<8, 8, 8);

	if ( angle>= 4 )
		flip_mirror = 1;
	else 
		flip_mirror = 0;


	if (hide)
	{
		x -= (RINK_CAMERA_X);
		y -= (RINK_CAMERA_Y);

		init_sprite( sprite, SIZE_8, SQUARE, HOCKEY_STICK_TILES, ang*2, stick_layer[angle], 0, flip_mirror, flip_mirror );
		move_sprite( sprite, x, y );
	
	} else {
		hide_sprite( sprite );
	}


}






