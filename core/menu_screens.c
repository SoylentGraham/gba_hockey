#include "menu_screens.h"
#include "gba.h"
#include "types.h"
#include "keypad.h"
#include "core.h"
#include "tile_alloc.h"
//#include "tiles.h"

#include "../game/game.h"
#include "../media/map/map_data.h"
#include "sprites.h"
#include "animated_sprite.h"



#define SPOTLIGHT_W 5
#define SPOTLIGHT_H 5

//const u8 spotlight [ SPOTLIGHT_W * SPOTLIGHT_H ] = {
u8 spotlight [ SPOTLIGHT_W * SPOTLIGHT_H ] = {
	2,  3,  4,  5,  6, 
	7,  0,  0,  0,  8, 
	9,  0,  0,  0,  10,
	11, 0,  0,  0,  12,
	13, 14, 15, 16, 17
};



extern a_sprite_anim sprite_animations[MAX_SPRITES];
extern a_sprite sprites[MAX_SPRITES];
extern const u8 tile_set_lengths[TILE_TYPE_COUNT];


/*

void draw_spotloght( u8 spotlight_x, u8 spotlight_y, u8 bg )
{
	u8 x,y;

//  #define XY_COORD(x,y) ( ( y*SPOTLIGHT_H) + SPOTLIGHT_W )

	for ( x=0; x<SPOTLIGHT_W; x++)
		for ( y=0; y<SPOTLIGHT_H; y++)
		{
			draw_tile( spotlight_x+x, spotlight_y+y, BG_TILE(BLANK_TILES),bg);

			if ( spotlight[ XY_ARRAY(x,y,SPOTLIGHT_W) ] != 0 )
				draw_tile( spotlight_x+x, spotlight_y+y, BG_TILE(SPOTLIGHT_TILES)+spotlight[ XY_ARRAY(x,y,SPOTLIGHT_W) ]-1,bg);
		}
}



//clear tilemaps, load tilesets etc
void reload_menu_gfx(void)
{
	hide_all_backgrounds;

	//clear tile sets
	clear_current_tiledata(BG);
	clear_current_tiledata(OBJ);
	
	load_tile_palette();

	scrollbackground( 0, 0, 0 );
	scrollbackground( 0, 0, 1 );
	scrollbackground( 0, 0, 2 );
	scrollbackground( 0, 0, 3 );

	set_text_boundries( 0, VISIBLE_TILE_H, 0, VISIBLE_TILE_W );

	REG_BLDMOD = TARGET_TOP_BG2 | TARGET_LOW_BG3 | TARGET_LOW_OBJ | BLEND_NEG;
	REG_COLEV = BLEND_LEVEL(8);

	//clear any text on screen
	load_tile_set(BG,BLANK_TILES);
	load_tile_set(BG,FONT_TILES);
	load_tile_set(BG,ROAD_TILES);
	load_tile_set(BG,BUILDING_TILES);
	load_tile_set(BG,SHADOW_TILES);
	load_tile_set(BG,SPOTLIGHT_TILES);
	//clear background 1 to black
	load_tile_set(OBJ,FONT_TILES);

	init_anims();
	init_sprites();

	reset_tilemap(0,0,road_map,title_screen_building_map,title_screen_shadow_map, 	3, 3, 2, TITLE_SCREEN_MAP_W, TITLE_SCREEN_MAP_H);
//	reset_tilemap(0,0,road_map,building_map, shadow_map, 3, 3, 2 );

	clear_tile_map( BG_TILE(BLANK_TILES)+1 , 1);

	//draw_background( 0 , 1);
	//blank all these
	clear_tile_map( BG_TILE(BLANK_TILES) , HUD_BACKGROUND);


	init_sprite( &sprites[0], SIZE_8,SQUARE,  FONT_TILES, 0, 0, 0, false, false);
	//move_sprite( &sprites[0], 10, 10);
	rotate_sprite(&sprites[0], 270,ZOOM(1),ZOOM(1));
	make_sprite_anim( &sprite_animations[0], &sprites[0], FONT_TILES, 0 , 0, tile_set_lengths[FONT_TILES]-1, true, 2);

	show_all_backgrounds;
}


void redraw_menu_text( a_menu_option_text *menu_text, u8 option_count, s8 selected_option )
{
	u8 m;
	for ( m=0; m<option_count; m++)
			draw_text(3,MENU_TOP+m,false,"%s",menu_text[m]); //er, dont need %s
//	draw_string(1,MENU_TOP+selected_option,false,"-"); //er, dont need %s
}



//a menu_function function
u8 make_menu(a_menu_option_text *menu_text,a_menu_function *menu_functions,u8 option_count,u8 *inital_param, u8 bg_tile_set_id)
{
	s8 selected_option=0;
	s8 old_selected_option=0;
	u8 selected_option_pos=PIXEL_POS_OF_OPTION(selected_option);
	u8 m,i,menu_result;

	//invlaid option number
	if (option_count==0 || option_count>=MAX_MENU_ITEMS)
		return LEFT_THE_MENU; 

	//reloads all generic required tiles/sprites for a menu
	reload_menu_gfx();

	//fills a bg with a whole graphic for a bg
	//draw_background( bg_tile_set[BG][ bg_tile_set_id ] , MENU_BACKGROUND_BACKGROUND );

	//draw each menu option [once; wont get over written]
	redraw_menu_text (menu_text, option_count, selected_option);

	while (1)
	{
		wait_for_vblank();

		//animate sprite/background not on the hud
		//process menu input
		if ( KEY_START || KEY_SELECT || KEY_A ) //selected an item
			//make sure is a menu option. this means i can have some text next to a menu option
			if ( menu_functions[selected_option] != null ) 
			{
				//call that menu_option func
				menu_result=((a_menu_function)menu_functions[selected_option])(&selected_option);
				//do somehting based on the menu result, like, go back or somehting

				//reload menu_screen tilesets. incase we went into a game or somehting that chnaged our tilesset's
				reload_menu_gfx();
				redraw_menu_text (menu_text, option_count, selected_option);
			}


		if (KEY_B)
			return LEFT_THE_MENU;

		//clear old menu option
		//old_selected_option = selected_option;

		if (old_selected_option == selected_option)
		{
			if (KEY_UP) //scrollup through selection
				selected_option--;
			if (KEY_DOWN) //scroll down
				selected_option++;
		}

		//if go past the bottom option, jumps back to the top and vice-versa
		//selected_option=roll_u8(selected_option,0,option_count); 
		if (selected_option<0)
			selected_option = 0;
		if (selected_option>=option_count)
			selected_option = option_count-1;



		//if this transition doesnt work, convert to fixed point
		
		//selected_option_pos+=(PIXEL_POS_OF_OPTION(selected_option)-selected_option_pos)/4;
		if ( PIXEL_POS_OF_OPTION(selected_option) > selected_option_pos)
			selected_option_pos+=1;
		else
			selected_option_pos-=1;

		//snap to final position, just in case above fails
		if ( selected_option_pos >= PIXEL_POS_OF_OPTION(selected_option)-2 )
			if ( selected_option_pos <= PIXEL_POS_OF_OPTION(selected_option)+2 )
			{
				selected_option_pos = PIXEL_POS_OF_OPTION(selected_option);
				old_selected_option = selected_option;
			}


		draw_spotloght( (VISIBLE_TILE_W/2)-(SPOTLIGHT_W/2)-1, (VISIBLE_TILE_H/2)-(SPOTLIGHT_H/2)-2, 1 );
		move_sprite( &sprites[0], 12, selected_option_pos);


		//draw the selected_option marker (sprite or tile..)
		//draw at selected_option_pos, moving to desired pos

		//redraw_menu_text (menu_text, option_count, selected_option);
		
		process_anims();
		update_sprites();
		wait_for_end_of_vblank();
	}



}


*/