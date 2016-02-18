#ifndef GAME_MENUS_H
#define GAME_MENUS_H


#include "../core/gba.h"
#include "../core/types.h"
#include "../core/core.h"
#include "../core/menu_screens.h"


#define MENUSTRLEN 14
#define ROLL_STAGES 4
#define ROLL_SCROLL_RATE 4
#define MENU_XSTART (0+MENUSTRLEN+2)
#define MENU_YSTART (2)


void init_menu(void);
void test_menu(void);



#endif

