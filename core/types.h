#ifndef CORE_TYPES_H
#define CORE_TYPES_H


#include "gba.h"



typedef s16 two_coords[2];


#define TRUE 1
#define FALSE 0
#define true TRUE
#define false FALSE
#define NULL 0
#define null NULL

#define X 0
#define Y 1


#define TILE_W 8
#define TILE_H 8
#define CAR_W 16
#define CAR_H 16

#define VISIBLE_SCREEN_W 240
#define VISIBLE_SCREEN_H 160

#define FULL_SCREEN_W 256
#define FULL_SCREEN_H 256

#define FULL_TILE_W (FULL_SCREEN_W / TILE_W)
#define FULL_TILE_H (FULL_SCREEN_H / TILE_H)

#define VISIBLE_TILE_W (VISIBLE_SCREEN_W / TILE_W)
#define VISIBLE_TILE_H (VISIBLE_SCREEN_H / TILE_H)



#define BG_MAX_TILES 255
#define OBJ_MAX_TILES 255




#define ROLL_SCREEN_NUMBER(xy) ( roll_s16(xy,0,FULL_TILE_W) )



#define XY_ARRAY(x,y,w) ( ( (y) *(w)) + (x) )

//macro to get bit(n)'s number
#define BIT(n) ( 1 << n ) // <--- i think. other wise make array[16] of bit numbers


#define OBJ 0
#define REAL_BG(n) (n)
#define BG(n) (1+(n==3?2:n))
//#define BG(n) (1+(n==3?1:n))




#define BG_PRIORITY(n) ( (n) * 0x1 )
#define PRIORITY(n)		((n) << 10)
#define ZOOM(n) ( (n) << 8 )
#define PALETTE(n)		((n) << 12)

#define BLEND_NEG 0x40
#define BLEND_LOW(n) ( (n) * 0x1 )
#define BLEND_HIGH(n) ( (n) * 0x100 )
#define BLEND_LEVEL(n) ( BLEND_LOW(n) | BLEND_HIGH(n) )





#define CHAR_BLOCK_REGISTER(n) ( (n+1) *0x4)
#define SCREEN_BLOCK_REGISTER(n) ( (n) *0x200)

#define CHAR_BASE_BLOCK(n) (REG_TILE_BASE + (0x4000 * (n+1)))
//#define CHAR_BASE_BLOCK(n) ((((n)+1)*0x4000)+REG_TILEMAP_BASE) //16k * number + start of VRAM = address of character base block
#define SCREEN_BASE_BLOCK(n) (((n)*0x800)+REG_TILEMAP_BASE) //2k * number + start of VRAM = address of screen base block

#define BG_MOSAIC 0x40
#define BG_COLOR_256(y) ( (y) *0x80)



#define ROAD_BACKGROUND 3
#define ROOFTOP_BACKGROUND 2
#define SHADOW_BACKGROUND 1
#define HUD_BACKGROUND 0




#define TEXTBG_SIZE_256x256 0x0
#define TEXTBG_SIZE_256x512 0x8000
#define TEXTBG_SIZE_512x256 0x4000
#define TEXTBG_SIZE_512x512 0xC000

#define ROTBG_SIZE_128x128 0x0
#define ROTBG_SIZE_256x256 0x4000
#define ROTBG_SIZE_512x512 0x8000
#define ROTBG_SIZE_1024x1024 0xC000

#define WRAPAROUND 0x2000

#define REG_CHAR_MAP_0 0x0
#define REG_CHAR_MAP_1 0x4
#define REG_CHAR_MAP_2 0x7
#define REG_CHAR_MAP_3 0x3



#define CHAR_SHIFT 2 //used to shift the number over to the right place in REG_BGxCNT

#define SCREEN_SHIFT 8



//mosaic stuffs
#define MOS_BG_HOR(n) (n)
#define MOS_BG_VER(n) (n<<4)
#define MOS_OBJ_HOR(n) (n<<8)
#define MOS_OBJ_VER(n) (n<<12)

#define SetMosaic(bh,bv,oh,ov) ((bh)+(bv<<4)+(oh<<8)+(ov<<12))

//usage REG_MOSAIC = SetMosaic(4,4,0,0); for a mosaic of 4x4 on the background




#define RAND_MAX 32767
volatile s32 RAND_RandomData;



#endif

