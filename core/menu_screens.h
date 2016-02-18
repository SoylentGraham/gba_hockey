#ifndef CORE_MENU_SCREENS
#define CORE_MENU_SCREENS


#include "gba.h"





#define MAX_MENU_ITEMS 7
#define MAX_MENU_WIDTH ((240/8) - 5) //in chars
#define MENU_TOP (20-1-MAX_MENU_ITEMS) //20 is screen height

#define LEFT_THE_MENU 0
#define PIXEL_POS_OF_OPTION(n) ((MENU_TOP*8)+(n*8))




typedef u8(*a_menu_function)(u8*);
typedef char a_menu_option_text[ MAX_MENU_WIDTH ]; //2 tile gaps on either side








u8 make_menu(a_menu_option_text *menu_text,a_menu_function *menu_functions,u8 option_count,u8 *inital_param, u8 bg_tile_set_id);






#endif


