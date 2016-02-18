#include "gba.h"
#include "core.h"
#include "types.h"
#include "tiles.h"




void wait_for_vblank(void)
{
	volatile u8 v_count=REG_VCOUNT;
	v_count = REG_VCOUNT;
	while (v_count >= 160)
	{
		v_count = (volatile u8)REG_VCOUNT;
	}
}

void wait_for_end_of_vblank(void)
{
	volatile u8 v_count=REG_VCOUNT;
	v_count = REG_VCOUNT;
	while (v_count < 160)
	{
		v_count = (volatile u8)REG_VCOUNT;
	}
}

int is_odd(int number)
{
	return ( number%2 ==1 );
}


s8 roll_s8(s8 number,s8 low, s8 high)
{
	while (number>=high)		number-=high;
	while (number<low)		number=high+number;
	return number;
}

u8 roll_u8(u8 number,u8 low, u8 high)
{
	while (number>=high)		number-=high;
	while (number<low)		number=high+number;
	return number;
}

s16 roll_s16(s16 number,s16 low, s16 high)
{
	while (number>=high)		number-=high;
	while (number<low)		number=high+number;
	return number;
}

u16 roll_u16(u16 number,u16 low, u16 high)
{
	while (number>=high)		number-=high;
	while (number<low)		number=high+number;
	return number;
}


s32 roll_s32(s32 number,s32 low, s32 high)
{
	while (number>=high)		number-=high;
	while (number<low)		number=high+number;
	return number;
}

u32 roll_u32(u32 number,u32 low, u32 high)
{
	while (number>=high)		number-=high;
	while (number<low)		number=high+number;
	return number;
}




void set_mode(u8 mode,u16 bgs,u8 sprites)
{
	REG_BG0CNT = CHAR_BLOCK_REGISTER(0) | BG_PRIORITY(0) | BG_MOSAIC | BG_COLOR_256(1) | SCREEN_BLOCK_REGISTER(0) ;
	REG_BG1CNT = CHAR_BLOCK_REGISTER(1) | BG_PRIORITY(1) | BG_MOSAIC | BG_COLOR_256(1) | SCREEN_BLOCK_REGISTER(1) ;
	REG_BG2CNT = CHAR_BLOCK_REGISTER(2) | BG_PRIORITY(2) | BG_MOSAIC | BG_COLOR_256(1) | SCREEN_BLOCK_REGISTER(2) ;
	REG_BG3CNT = CHAR_BLOCK_REGISTER(2) | BG_PRIORITY(3) | BG_MOSAIC | BG_COLOR_256(1) | SCREEN_BLOCK_REGISTER(3) ;

	REG_DISPCNT = BG_MODE(mode) | OBJ_1D_MAP;
	REG_DISPCNT |= bgs;
	if (sprites)
		REG_DISPCNT |= OBJ_ENABLE;

	//REG_DISPCNT = BG_MODE_0 | OBJ_1D_MAP | OBJ_ENABLE | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE ;

	scrollbackground( 0, 0, HUD_BACKGROUND );
	scrollbackground( 0, 0, ROAD_BACKGROUND );
	scrollbackground( 0, 0, ROOFTOP_BACKGROUND );
	scrollbackground( 0, 0, SHADOW_BACKGROUND );
}

