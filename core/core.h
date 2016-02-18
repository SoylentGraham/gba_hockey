#ifndef CORE_H
#define CORE_H


#include "gba.h"



void wait_for_vblank(void);
void wait_for_end_of_vblank(void);
int is_odd(int number);
s8 roll_s8(s8 number,s8 low, s8 high);
u8 roll_u8(u8 number,u8 low, u8 high);
s16 roll_s16(s16 number,s16 low, s16 high);
u16 roll_u16(u16 number,u16 low, u16 high);
s32 roll_s32(s32 number,s32 low, s32 high);
u32 roll_u32(u32 number,u32 low, u32 high);
void set_mode(u8 mode,u16 bgs,u8 sprites);


#define hide_all_backgrounds (set_mode(0,0,0))
#define show_all_backgrounds (set_mode(0,BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE,1))



#endif

