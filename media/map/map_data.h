#ifndef MAP_DATA_H
#define MAP_DATA_H


#include "../../core/gba.h"



//space the actual play area takes up
//48 is max before breaks. 47 is max before counter breaks (wont show) and 46 max before fps=0
//#define ICE_RINK_W  46
//#define ICE_RINK_H	ICE_RINK_W
#define ICE_RINK_W  34
#define ICE_RINK_H	45
//crowd size
#define RINK_EDGE_WIDTH 5

#define MEDIA_MAP_W ( ICE_RINK_W+ (RINK_EDGE_WIDTH*2) )
#define MEDIA_MAP_H ( ICE_RINK_H+ (RINK_EDGE_WIDTH*2) )


const u16 def_rink_floor[ MEDIA_MAP_W * MEDIA_MAP_H ];
const u16 def_rink_screen[ MEDIA_MAP_W * MEDIA_MAP_H ];
const u16 def_rink_overhead[ MEDIA_MAP_W * MEDIA_MAP_H ];



#endif

