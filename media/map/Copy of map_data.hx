#ifndef MAP_DATA_H
#define MAP_DATA_H


#include "../../core/gba.h"


#define MEDIA_MAP_W 100
#define MEDIA_MAP_H 100



const u8 shadow_map[ (((MEDIA_MAP_W*3)/2)*3) *(MEDIA_MAP_H*3) ] ;
const u8 building_map[ (MEDIA_MAP_W*3)*(MEDIA_MAP_H*3) ] ;
const u8 road_map[ (MEDIA_MAP_W/2)*MEDIA_MAP_H ] ;




#endif

