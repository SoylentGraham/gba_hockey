#include "gba.h"
#include "types.h"
#include "sprites.h"
#include "tile_alloc.h"
#include "animated_sprite.h"


a_sprite_anim sprite_animations[MAX_SPRITES];

extern u8 sprite_anim_count;



void init_anims(void)
{
	u8 i;
	//for (i=0; i<MAX_SPRITES; i++)
	for (i=0; i<sprite_anim_count; i++)
		end_sprite_anim( &sprite_animations[i] );
}


void process_anims(void)
{
	u8 i;
	for (i=0; i<MAX_SPRITES; i++)
		process_sprite_anim( &sprite_animations[i] );
}



void end_sprite_anim( a_sprite_anim *anim)
{
	anim->sprite=0;
}



void make_sprite_anim( a_sprite_anim *anim, a_sprite *sprite, u8 obj_set, u8 start_frame, u8 tile_skip, u8 loop_length, u8 looping, u8 frame_delay)
{
	anim->sprite = sprite;
	
	anim->obj_set = obj_set;
	anim->first_frame = start_frame;
	anim->last_frame = start_frame + loop_length -1;

	anim->tile_skip = tile_skip;
	anim->looping = looping;
	anim->paused = false;
	anim->delay_counter = 0;
	anim->frame_delay = frame_delay;

	
	//oddly, doing this once, breaks it. it has to be set twice, before anything happens...
	force_anim_frame(anim, 0);
	force_anim_frame(anim, 0);
}

void un_pause_anim(a_sprite_anim *anim)
{
	anim->paused=false;
}

u8 get_sprite_anim_frame(a_sprite_anim *anim)
{
	u16 tile;
	tile = get_sprite_tile( anim->sprite);
	tile -= bg_tile_set[OBJ][ anim->obj_set ];


	tile = tile / (anim->tile_skip);
	tile -= anim->first_frame;

	return (u8)tile;
}


void force_anim_frame(a_sprite_anim *anim, u8 frame)
{
	u16 new_frame;
	
	new_frame =  bg_tile_set[OBJ][ anim->obj_set ]  + ( anim->tile_skip * (anim->first_frame+frame) );
	new_frame *= 2;

	set_sprite_tile(anim->sprite, new_frame );
}


void process_sprite_anim(a_sprite_anim *anim)
{
	u8 current_frame;

	if (!anim->sprite) return; //remove this pointer to disable anim
	//extract the current tile from the actual sprite
	current_frame = get_sprite_anim_frame( anim );
	

	if (!anim->paused)
		anim->delay_counter++;

	if ( anim->delay_counter >= anim->frame_delay) //next frame
	{
		anim->delay_counter = 0;
		
		current_frame += 1;
		//current_tile += anim->tile_skip;


		if (current_frame > anim->last_frame - anim->first_frame )
		{
			if (!anim->looping) //stop anim on the last frame
			{
				end_sprite_anim(anim);
				return; //done all i need to seeing as we've finished looping once
			}
			
			current_frame = 0;
		}


		/*
		if ( current_tile > OBJ_TILE(anim->obj_set,anim->end_obj_tile) ) //got to end of frames
		{
			if (!anim->looping) //stop anim on the last frame
			{
				end_sprite_anim(anim);
				return; //done all i need to seeing as we've finished looping once
			}
			current_tile = OBJ_TILE( anim->obj_set, anim->obj_tile) ;  //go back to start frame
		}
		*/

		//set sprite to the new frame (0..frame_len)
		force_anim_frame(anim,current_frame);
	}
}


