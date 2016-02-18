
#include "gba.h"
#include "types.h"
#include "map.h"


#include "tile_alloc.h"
#include "../game/tile_sets.h"

extern const byte building_map[ (MAP_W*3)*(MAP_H*3) ] ;
extern const byte road_map[ (MAP_W/2)*MAP_H ] ;
extern const byte shadow_map[ (((MAP_W*3)/2)*3) *(MAP_H*3) ];



extern const u8 tile_set_lengths[TILE_TYPE_COUNT];

/*

//returns number between 0 and 15
byte get_road_data(const u8 *map, s16 x, s16 y,s16 map_w,s16 map_h)
{
	//byte tmp,result;
	byte result;
	s16 real_x;
	s16 real_y;

	real_x= ( x/3 ) ;
	real_y= ( y/3 ) ;

	int pos=( ( real_y ) * (map_w/2) ) + ( real_x/2  );

	//real_x and real_y arent less than 0 when 0/2  for some reason...
	if ( x<0 || y<0 || real_x >= map_w || real_y >= map_h )
		return 0;
 



	if (! is_odd(real_x) ) //even
	{
		result=((a_road_tile*)&map[pos])->road_tile_a;
	} else {
		result=((a_road_tile*)&map[pos])->road_tile_b;
	}



	return result;

}



//uses road number (0-15)
byte get_road_tile(const u8 *map, s16 x, s16 y,s16 map_w,s16 map_h)
{
	byte remain[2];
	//number between 0 and 15
	u8 road_id;

	remain[X]= roll_u8(x,0,3) % 3 ;
	remain[Y]= roll_u8(y,0,3) % 3 ;

	road_id=get_road_data( map, x-remain[X] , y-remain[Y], map_w, map_h );

	//road_id=1;
	
	return ( BG_TILE(ROAD_TILES) + 
		(road_id*9)  +
		(remain[Y]*3) +
		(remain[X]) 
		);
}


u8 get_building_tile_data(const u8 *map, s16 x, s16 y, s16 map_w,s16 map_h)
{
	u8 b_t;
	if (!coord_in_map(x,y,MAP_W*3,MAP_H*3))
		return 0;

	b_t = ((a_building_tile*)&map[ XY_ARRAY( x,y, map_w*3) ]) ->building_tile_n;
	
	if (b_t > tile_set_lengths[BUILDING_TILES] )
	{
		b_t += 196;
		b_t -= 53;

//		b_t -= (BG_TILE(BUILDING_TILES)-1);
//		b_t += BG_TILE(ROAD_TILES);
	}

	return b_t;
}


byte get_building_tile(const u8 *map, s16 x, s16 y, s16 map_w,s16 map_h)
{
	u8 n = get_building_tile_data(map, x, y,map_w,map_h);

	if ( n==0 )
		return BG_TILE(BLANK_TILES);

	return ( n + BG_TILE(BUILDING_TILES) - 1 );
}


byte get_shadow_tile(const u8  *map, s16 x, s16 y,s16 map_w,s16 map_h)
{
	if (x<0) return BG_TILE(BLANK_TILES);
	if (y<0) return BG_TILE(BLANK_TILES);
	if (x>=map_w*3) return BG_TILE(BLANK_TILES);
	if (y>=map_h*3) return BG_TILE(BLANK_TILES);


	byte use_shadow_1= (x & 1);
	
	s16 shadow_x=	( x - use_shadow_1 ) / 2 ;
	s16 shadow_n= ( y * ((map_w*3)/2) ) + shadow_x;


	byte shadow_val;
	
	if ( use_shadow_1 )
		shadow_val= ((a_shadow_tile*)&map[ shadow_n ])->shadow_1;
	else
		shadow_val= ((a_shadow_tile*)&map[ shadow_n ])->shadow_0;

	if (shadow_val==0) 
		return BG_TILE(BLANK_TILES);
	
	if (shadow_val==1) //blank, all black tile
		return BG_TILE(BLANK_TILES)+ALL_BLACK_TILE;

	return (BG_TILE(SHADOW_TILES) + shadow_val -2 );

}



//tiles
u8 coord_in_map(s16 x, s16 y, u16 w, u16 h)
{
	if ( x<0 || y<0 || x>=w || y>=h )
		return false;
	return true;
}



//reloads all applicable backgrounds (road/roof/shadows)
void reload_column(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y, 
				   u8 *this_road_map, u8* this_building_map, u8* this_shadow_map, 
				   u8 road_bg, u8 building_bg, u8 shadow_bg, s16 map_w, s16 map_h	)
{
	//map_x & map_x is the top of the column
	s16 y;
	u16 s_x,s_y;

	for ( y=0; y<FULL_TILE_H; y++)
	{
		s_x = ROLL_SCREEN_NUMBER( screen_x ) ;
		s_y = ROLL_SCREEN_NUMBER( screen_y + y ) ;

		if (this_road_map)
			draw_tile( s_x, s_y, get_road_tile( (this_road_map), map_x, map_y+y, map_w, map_h) , road_bg );
		if (this_building_map)
			draw_tile( s_x, s_y, get_building_tile( (this_building_map), map_x, map_y+y, map_w, map_h) , building_bg ); 
		if (this_shadow_map)
			draw_tile( s_x, s_y, get_shadow_tile( (this_shadow_map), map_x, map_y+y, map_w, map_h) , shadow_bg );
	}	

	s_x = ROLL_SCREEN_NUMBER( screen_x ) ;
	s_y = ROLL_SCREEN_NUMBER( screen_y + 31 ) ;

	if (this_road_map)
		draw_tile( s_x, s_y, get_road_tile( (this_road_map), map_x, map_y-1, map_w, map_h) , road_bg );
	if (this_building_map)
		draw_tile( s_x, s_y, get_building_tile( (this_building_map) , map_x, map_y-1, map_w, map_h) , building_bg );
	if (this_shadow_map)
		draw_tile( s_x, s_y, get_shadow_tile( (this_shadow_map) , map_x, map_y-1, map_w, map_h) , shadow_bg );

}


//same but with rows

void reload_row(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y,
				   u8 *this_road_map, u8* this_building_map, u8* this_shadow_map, 
				   u8 road_bg, u8 building_bg, u8 shadow_bg, s16 map_w, s16 map_h	)
{	

	//map_x & map_x is the top of the column
	s16 x;
	u16 s_x,s_y;

	for ( x=0; x< FULL_TILE_W; x++)
	{
		s_x = ROLL_SCREEN_NUMBER( screen_x + x ) ;
		s_y = ROLL_SCREEN_NUMBER( screen_y ) ;

		if (this_road_map)
			draw_tile( s_x, s_y, get_road_tile( (this_road_map) , map_x+x, map_y, map_w, map_h), road_bg);
		if (this_building_map)	
			draw_tile( s_x, s_y, get_building_tile( (this_building_map) , map_x+x, map_y, map_w, map_h) , building_bg );
		if (this_shadow_map)
			draw_tile( s_x, s_y, get_shadow_tile( (this_shadow_map) , map_x+x, map_y, map_w, map_h) , shadow_bg);

	}

	s_x = ROLL_SCREEN_NUMBER(screen_x+31);
	s_y = ROLL_SCREEN_NUMBER(screen_y);

	if (this_road_map)
		draw_tile( s_x , s_y, get_road_tile( (this_road_map) , map_x-1, map_y, map_w, map_h) , road_bg);
	if (this_building_map)	
		draw_tile( s_x , s_y, get_building_tile( (this_building_map) , map_x-1, map_y, map_w, map_h) , building_bg); 
	if (this_shadow_map)
		draw_tile( s_x , s_y, get_shadow_tile( (this_shadow_map) , map_x-1, map_y, map_w, map_h) , shadow_bg);

}


void reload_game_row(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y)
{
	reload_row(screen_x, screen_y, map_x, map_y,  road_map, building_map, shadow_map, ROAD_BACKGROUND, ROOFTOP_BACKGROUND, SHADOW_BACKGROUND, MAP_W, MAP_H);
}

void reload_game_column(s16 screen_x, s16 screen_y, s16 map_x, s16 map_y)
{
	reload_column(screen_x, screen_y, map_x, map_y,  road_map, building_map, shadow_map, ROAD_BACKGROUND, ROOFTOP_BACKGROUND, SHADOW_BACKGROUND, MAP_W, MAP_H);
}


void reset_game_tilemap(s16 x_start, s16 y_start)
{
	reset_tilemap(x_start,y_start, 
		road_map, building_map, shadow_map, ROAD_BACKGROUND, ROOFTOP_BACKGROUND, SHADOW_BACKGROUND, MAP_W, MAP_H);
}


void reset_tilemap(s16 x_start, s16 y_start, 
				   u8 *this_road_map, u8* this_building_map, u8* this_shadow_map, 
				   u8 road_bg, u8 building_bg, u8 shadow_bg, s16 map_w, s16 map_h	)
{
	s16 x, y;
	byte tile;

	for (x = 0; x < FULL_TILE_W; x ++)
	{
		reload_column( roll_s16(x+x_start,0,32) ,0, x+x_start, y_start,
			this_road_map, this_building_map, this_shadow_map, road_bg, building_bg, shadow_bg , map_w, map_h);
		//tile_n++;
		//if (tile_n >= TILE_WIDTH*TILE_HEIGHT)	tile_n=0;
	}
	reload_column( roll_s16(31+x_start,0,32) ,0, x_start-1, y_start,
		this_road_map, this_building_map, this_shadow_map, road_bg, building_bg, shadow_bg, map_w, map_h );

}

*/