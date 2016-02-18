#include "../core/gba.h"
#include "../core/types.h"
#include "../core/core.h"
#include "../core/keypad.h"
#include "../core/menu_screens.h"
#include "../core/tile_alloc.h"

#include "menus.h"
#include "game.h"
#include "tile_sets.h"



extern u16 sys_brightness_mod;
//#define MAX_BRIGHTNESS_MOD (u16)40
//#define MIN_BRIGHTNESS_MOD (u16)0



u8 start_game(u8 *param)
{
	init_game();
	while (game_loop());
}





void init_menu(void)
{
	hide_all_backgrounds;
	set_mode(0, BG0_ENABLE, false );
	
	REG_BLDMOD = 0;
	//REG_BLDMOD = TARGET_TOP_BG1 | TARGET_LOW_BG2 | TARGET_LOW_BG1 | TARGET_LOW_BG3 | TARGET_LOW_OBJ | BLEND_NEG;
	//REG_COLEV = BLEND_LEVEL(8);

	clear_current_tiledata(BG(0));
	load_tile_palette(sys_brightness_mod);
	
	//tiles i need for my hud (HUD_BG)
	load_tile_set(BG(HUD_BG),	BLANK_TILES);
	load_tile_set(BG(HUD_BG),	FONT_TILES);
	load_tile_set(BG(HUD_BG),	FONT_SCROLL_1_TILES);
	load_tile_set(BG(HUD_BG),	FONT_SCROLL_2_TILES);
	load_tile_set(BG(HUD_BG),	FONT_SCROLL_3_TILES);

	set_text_boundries( 0, VISIBLE_TILE_H-1, 0, VISIBLE_TILE_W );

	init_anims();
	init_sprites(); //hides em all

	scrollbackground( 0, 0, 0 );

	clear_tile_map( BG_TILE(0,BLANK_TILES), 0);
}


const stage_tilesets[ROLL_STAGES] = {	FONT_TILES,FONT_SCROLL_1_TILES,FONT_SCROLL_2_TILES,FONT_SCROLL_3_TILES};

enum {
	OLD_KEY_UP=0,
	OLD_KEY_DOWN,
	OLD_KEY_LEFT,
	OLD_KEY_RIGHT,
	OLD_KEY_A,
	OLD_KEY_B,
	OLD_KEY_COUNT,
};

#define KEY_PRESS(k,o) ( (k)&& !was_down[o] )

void draw_menu(u8 mencount, u8 *optcount, char *mentexts, char **opttexts, u8 *menoptchoices )
{
	u8 *roller_val = 0;

	u8 finaltile,tile,set;
	u8 aim,i,m;
	u8 arrayn;
	u8 selectedopt=0;
	char txtbuff[MENUSTRLEN+1];
	memset( txtbuff, 0, MENUSTRLEN+1 );
	u8 was_down[OLD_KEY_COUNT]={0};
//	roller_val = (u8*)malloc( (mencount*MENUSTRLEN) * sizeof(u8) );


	for (i=0;i<mencount;i++)
	{
		memcpy( &txtbuff, &(mentexts[ XY_ARRAY(0,i,MENUSTRLEN) ]), MENUSTRLEN );
		draw_text(2,MENU_YSTART+i,0,"%s",txtbuff );
		//draw_text(2,2+i,0,mentexts[0][i] );
	//	opttexts[i][menoptchoices[i]]
	}


	while(1)
	{
		start_frame();

		for ( m=0; m<mencount; m++ )
		{
			for ( i=0; i<MENUSTRLEN; i++ )
			{
				//array coord of the roll data's
				arrayn = XY_ARRAY(i,m,MENUSTRLEN);
				
				//grab relative tile number of this char
				aim = get_font_tile(opttexts[m][ XY_ARRAY(i,menoptchoices[m],MENUSTRLEN) ], 0) * ROLL_STAGES;
				
				txtbuff[0] = opttexts[m][ XY_ARRAY(i,menoptchoices[m],MENUSTRLEN) ];
				aim = get_font_tile(txtbuff[0],0);

				//transition to the aim char
				//roller_val[arrayn] += ( aim - roller_val[arrayn] ) / ROLL_SCROLL_RATE;
				roller_val[arrayn] = aim;

				//step in the roll (defines which tileset to take from)
				set = roller_val[arrayn] % ROLL_STAGES;
				set = 0;

				//actual tile in the tileset (which font char)
				tile = ( roller_val[arrayn] - set ) /ROLL_STAGES;
				tile = aim;

				//calculate the tile (NOTE! make generic func for this from the drawtext() funcs)
				finaltile = BG_TILE(REAL_BG(HUD_BG),stage_tilesets[ set ]) + tile ;

				//stick the tile onto the BG (NOTE! save row of which tiles and copy to bg)
				draw_tile( MENU_XSTART+i, MENU_YSTART+m, finaltile, REAL_BG(HUD_BG) );

			}
			if (m==selectedopt)
				draw_text(0,MENU_YSTART+m,0,"O");
			else
				draw_text(0,MENU_YSTART+m,0,"X");

		}


		if ( KEY_PRESS(KEY_LEFT,OLD_KEY_LEFT) || KEY_PRESS(KEY_B,OLD_KEY_B) )
			if ( menoptchoices[selectedopt]+1 < optcount[selectedopt] )
				menoptchoices[selectedopt]++;

		if ( KEY_PRESS(KEY_RIGHT,OLD_KEY_RIGHT) || KEY_PRESS(KEY_A,OLD_KEY_A) )
			//if ( optfuncs[selectedopt] )
			//	optfuncs[selectedopt]();
			//else
				if ( menoptchoices[selectedopt]-1 >= 0  )
					menoptchoices[selectedopt]--;

		if ( KEY_PRESS(KEY_UP,OLD_KEY_UP) )
				selectedopt--;
		if ( KEY_PRESS(KEY_DOWN,OLD_KEY_DOWN) )
				selectedopt++;


		if ( was_down[ OLD_KEY_UP ] != (KEY_UP!=0) )			was_down[ OLD_KEY_UP ] = (KEY_UP!=0);
		if ( was_down[ OLD_KEY_DOWN ] != (KEY_DOWN!=0) )			was_down[ OLD_KEY_DOWN ] = (KEY_DOWN!=0);
		if ( was_down[ OLD_KEY_LEFT ] != (KEY_LEFT!=0) )			was_down[ OLD_KEY_LEFT ] = (KEY_LEFT!=0);
		if ( was_down[ OLD_KEY_RIGHT ] != (KEY_RIGHT!=0) )			was_down[ OLD_KEY_RIGHT ] = (KEY_RIGHT!=0);
		if ( was_down[ OLD_KEY_A ] != (KEY_A!=0) )			was_down[ OLD_KEY_A ] = (KEY_A!=0);
		if ( was_down[ OLD_KEY_B ] != (KEY_B!=0) )			was_down[ OLD_KEY_B ] = (KEY_B!=0);


		//if ( KEY_B )	break;

		end_frame();
	}

//	free(roller_val);
	end_frame();
}


void test_menu(void)
{
	u8 mencount=4;
	char mentexts[4][MENUSTRLEN]		= { "home          ", "away         ", "time          ", "play          " };
	char homeawaytexts[3][MENUSTRLEN]	= { "teama         ", "teamb        ", "teamc         " };
	char timetexts[2][MENUSTRLEN]		= { "5             ", "10           " };
	char playtxt[MENUSTRLEN]			= { "....          " };
	char **opttexts[4]	= { &homeawaytexts, &homeawaytexts, &timetexts, &playtxt };
	u8 optcount[4]		= { 3, 3, 2, 0 };
	//func optfuncs[mencount]		= { 0, 0, 0, &play_func };
	u8 menoptchoices[4]	= { 0, 1, 0, 0 };

	init_menu();
	draw_menu(mencount,optcount,mentexts,opttexts,menoptchoices);
}




