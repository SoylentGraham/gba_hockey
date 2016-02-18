#ifndef CORE_KEYPAD_H
#define CORE_KEYPAD_H

#include "gba.h"


#define KEYS *(volatile u32*)0x04000130
#define KEY(x)     (!(KEYS & (x) ))

#define KEY_A 		KEY(1)
#define KEY_B 		KEY(2)
#define KEY_SELECT	KEY(4)
#define KEY_START 	KEY(8)
#define KEY_RIGHT 	KEY(16)
#define KEY_LEFT 	KEY(32)
#define KEY_UP 		KEY(64)
#define KEY_DOWN 	KEY(128)
#define KEY_R		KEY(256)
#define KEY_L 		KEY(512)






#endif

