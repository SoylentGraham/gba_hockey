#include "gba.h"
#include "core.h"
#include "collisions.h"
#include "map.h"
#include "../media/misc/car_mask_data.h"
#include "../media/gfx/media.h"


//const u16 car_mask [ CAR_MASK_COUNT ] [ CAR_MASK_W * CAR_MASK_H ];
const u8 tile_mask[ TILE_W * TILE_H ]={15}; //all collidable

extern const u16 car_mask [ CAR_MASK_COUNT ] [ CAR_MASK_W * CAR_MASK_H ];
//extern const u16 tile_mask[ TILE_W * TILE_H ]; //all collidable



#define OUTSIDE_ARRAY 63//(8*7)+7 //XY_ARRAY( 7, 7, 8)


u8 mask_on_mask_check( u8 *collision_bit_a ,u8 *collision_bit_b ,
						u8 x_a, u8 y_a, u8 x_b, u8 y_b,	const u8 *mask_a, const u8 *mask_b,
						u8 w_a, u8 w_b )
{
	*collision_bit_a = mask_a [ XY_ARRAY( x_a, y_a, w_a) ];

	*collision_bit_b = mask_b [ XY_ARRAY( x_b, y_b, w_b) ];

	return (*collision_bit_b & *collision_bit_a );
}




//not quick/easy to calc if angle is signed, and itll be done somewhere else anyways
void get_mask_n ( u16 angle, byte *mask_n, byte *mask_mode )
{
	*mask_n = roll_u16( ( angle +2 ) / 5, 0, 71 );
}




#define PIXEL_JUMP 1
#define COLLISION_LOOP_LIMIT 2 //4 layers in (*2 layers)
#define CHECK_MASK(xoff,yoff) (mask_on_mask_check( crash_id_a ,crash_id_b , xoff + overlap_x_a, yoff + overlap_y_a, xoff + overlap_x_b, yoff + overlap_y_b, mask_a, mask_b, width_a, width_b) )

#define FILL_U8(a,b,c,d,e,f,g,h) ( ((a!=0)<<0) | ((b!=0)<<1) | ((c!=0)<<2) | ((d!=0)<<3) | ((e!=0)<<4) | ((f!=0)<<5) | ((g!=0)<<6)  | ((h!=0)<<7) )
//#define FILL_U8(h,g,f,e,d,c,b,a) ( ((a!=0)<<0) | ((b!=0)<<1) | ((c!=0)<<2) | ((d!=0)<<3) | ((e!=0)<<4) | ((f!=0)<<5) | ((g!=0)<<6)  | ((h!=0)<<7) )
//#define FILL_U8(h,g,f,e,d,c,b,a) ( ((a)<<0) | ((b)<<1) | ((c)<<2) | ((d)<<3) | ((e)<<4) | ((f)<<5) | ((g)<<6)  | ((h)<<7) )
//#define FILL_U8(a,b,c,d,e,f,g,h) ( ((a)<<0) | ((b)<<1) | ((c)<<2) | ((d)<<3) | ((e)<<4) | ((f)<<5) | ((g)<<6)  | ((h)<<7) )




u8 check_mask_overlap_row( u8* crash_id_a, u8* crash_id_b, const u8 *mask_a, const u8 *mask_b,	u8 y,	u8 width,	u8 left,	u8 overlap_x_a,	u8 overlap_x_b,	u8 overlap_y_a,	u8 overlap_y_b,	u8 width_a,	u8 width_b , const u8* mask_u8_a, const u8* mask_u8_b)
{
	u16 array_n[2] ={0};
	u8 line_check[2] ={0};
	u8 x =0;

	array_n[0] = XY_ARRAY( left + overlap_x_a, y + overlap_y_a, width_a );
	array_n[1] = XY_ARRAY( left + overlap_x_b, y + overlap_y_b, width_b );
	
	if (mask_u8_a)	{
	//if (0)	{
		//look up pre-compiled
		line_check[0] = mask_u8_a[ XY_ARRAY( 0, y + overlap_y_a, width_a ) ] >> overlap_x_a;;
	} else { 
		//generate mask checker
		line_check[0] = FILL_U8( mask_a[array_n[0]+0] , mask_a[array_n[0]+1 ], mask_a[array_n[0]+2 ], mask_a[array_n[0]+3], mask_a[array_n[0]+4], mask_a[array_n[0]+5], mask_a[array_n[0]+6], mask_a[array_n[0]+7] );
	}


	//if(0) {
	if (mask_u8_b)	{
		//look it up
		line_check[1] = mask_u8_b[ XY_ARRAY( 0, y + overlap_y_b, width_b ) ] >> overlap_x_a;
	} else {
		//generate our mask checker
		line_check[1] = FILL_U8( mask_b[array_n[1]+0] , mask_b[array_n[1]+1 ], mask_b[array_n[1]+2 ], mask_b[array_n[1]+3], mask_b[array_n[1]+4], mask_b[array_n[1]+5], mask_b[array_n[1]+6], mask_b[array_n[1]+7] );
	}


	line_check[0] &= (1<<(width))-1;
	line_check[1] &= (1<<(width))-1;


	if (line_check[0] & line_check[1]) //hit on this row
	{
		x=left;
		while(x<width)
		{
			//if (CHECK_MASK(x,y))
			//	return true;

			if ( (*crash_id_a = mask_a [ array_n[0] + x ])!=0 )
				if ( (*crash_id_b = mask_b [ array_n[1] + x ])!=0 )
					if ( *crash_id_a & *crash_id_b )
						return true;
				
			x++;
		}
	}


	return false;
}









// checks collisions for two masks at relative positions. returns tr/fa and fills crash id's (where on mask collided)
u8 check_collisions ( u8 width_a, u8 width_b, u8 height_a, u8 height_b,
			 const u8 *mask_a, const u8 *mask_b, s16 x_coord_a, s16 y_coord_a, s16 x_coord_b, s16 y_coord_b,
			  u8 *crash_id_a, u8 *crash_id_b, const u8 *u8_mask_a, const u8 *u8_mask_b  ) 
{


	u8 overlap_x_a,overlap_x_b,overlap_y_a,overlap_y_b;
	u8 overlap_width,overlap_height;
	//loop nums
	u8 left,top,width,height; //boundries
	u16 loop_no;



	//if no chance of overlap, return
	if ( x_coord_a > x_coord_b + width_b ) return false;
	if ( x_coord_a + width_a < x_coord_b ) return false;
	if ( x_coord_b > x_coord_a + width_a ) return false;
	if ( x_coord_b + width_b < x_coord_a ) return false;

	if ( y_coord_a > y_coord_b + height_b ) return false;
	if ( y_coord_a + height_a < y_coord_b ) return false;
	if ( y_coord_b > y_coord_a + height_a ) return false;
	if ( y_coord_b + height_b < y_coord_a ) return false;


	

	//calculate start of overlaps for each mask. 
	//the different versions per a/b mean we dont have lots of ifs later on

#define MIN_OVERLAP_W 0
#define MIN_OVERLAP_H MIN_OVERLAP_W

	if ( x_coord_a < x_coord_b )
	{
		overlap_x_b = 0;
		overlap_x_a = ( x_coord_b - x_coord_a ) ;
	} else {
		overlap_x_b = ( x_coord_a - x_coord_b ) ;
		overlap_x_a = 0;
	}


	if ( y_coord_a < y_coord_b )
	{
		overlap_y_b = 0;
		overlap_y_a = ( y_coord_b - y_coord_a ) ;
	} else {
		overlap_y_b = ( y_coord_a - y_coord_b ) ;
		overlap_y_a = 0;
	}



	//pick smallest width
	if ( width_a < width_b )
		overlap_width = width_a;
	else 
		overlap_width = width_b;
	//offset width to not cover non-existant pixels
	if ( x_coord_a < x_coord_b )
		overlap_width = overlap_width - overlap_x_a;
	else
		overlap_width = overlap_width - overlap_x_b;



	//pick smallest height
	if ( height_a < height_b )
		overlap_height = height_a;
	else overlap_height = height_b;
	//offset height to not cover non-existant pixels
	if ( y_coord_a < y_coord_b )
		overlap_height = overlap_height - overlap_y_a;
	else
		overlap_height = overlap_height - overlap_y_b;
	


	if ( overlap_width < MIN_OVERLAP_W )
		return false;
	if ( overlap_height < MIN_OVERLAP_H )
		return false;


	//instead of going left/right/top/down; loop around the mask and go towards the center (more accurate!)
	left = 0 ;
	width = overlap_width ;
	top = 0 ;
	height= overlap_height ;

	

/*

	y=loop[Y]=loop[X]=0;
//	while ( loop[Y]<height )
//	while ( y<height )
	while ( y<8 )
	{
		//center outwards
		//y = (height/2) + up_down_direction[loop[Y]&1]*((loop[Y] + (loop[Y]&1))/2);

		line_check[0] = 0;
		line_check[1] = 0;

		//left of the overlapping mask area current row
		array_n[0] = XY_ARRAY( overlap_x_a, y+overlap_y_a, width_a );
		array_n[1] = XY_ARRAY( overlap_x_b, y+overlap_y_b, width_b );

		loop[X]=0;
	//	if (0)
		x=0;
		while (x<8)
		{
			line_check[0] = (mask_a[ array_n[0]+x ] != 0);
			line_check[1] = (mask_b[ array_n[1]+x ] != 0);
			
			draw_text(6+x, 6+y, 0, "%1d", 9 );
			if ( x<width && y<height )
				draw_text(6+x, 6+y, 0, "%1d", line_check[1] & line_check[0] );

			x++;
		}
		


		loop[X]=0;
	//	if (0)
		while (loop[X]<width)
		{
			//x=((loop[X]&1)*width) + ( up_down_direction[ loop[X]&1 ] * ((loop[X]-(loop[X]&1))/2) );
			x=loop[X];
			line_check[0] |= (mask_a[ array_n[0]+loop[X] ] != 0 )<<x;
			line_check[1] |= (mask_b[ array_n[1]+loop[X] ] != 0 )<<x;
		//	line_check[0] |= mask_a[ array_n[0]+loop[X] ] <<0;
		//	line_check[1] |= mask_b[ array_n[1]+loop[X] ] <<0;
			loop[X]++;
		}


//		line_check[0] = FILL_U8( mask_a[array_n[0]+0], mask_a[array_n[0]+1], mask_a[array_n[0]+2], mask_a[array_n[0]+3], mask_a[array_n[0]+4], mask_a[array_n[0]+5], mask_a[array_n[0]+6], mask_a[array_n[0]+7] );
//		line_check[1] = FILL_U8( mask_a[array_n[1]+0], mask_a[array_n[1]+1], mask_a[array_n[1]+2], mask_a[array_n[1]+3], mask_a[array_n[1]+4], mask_a[array_n[1]+5], mask_a[array_n[1]+6], mask_a[array_n[1]+7] );
//		line_check[0] &= (1<<width)-1;
//		line_check[1] &= (1<<width)-1;




		//collision somewhere on the line
		if ( line_check[0] & line_check[1] )
		{
			*crash_id_a = 0;
			*crash_id_b = 0;
			
			//goto get_mask_collision_values;
			x=0;
			loop[X]=0;
			
			x=left;
			while (x<width)
			{
				if (CHECK_MASK(x,y))
					return true;
				x++;
			}

			
			//while (x<width)
			if (0)
			while (loop[X]<width)
			{
				x=((loop[X]&1)*width) + ( up_down_direction[ loop[X]&1 ] * ((loop[X]-(loop[X]&1))/2) );

				if ( (line_check[0]>>x) & 1 )
					if ( (line_check[1]>>x) & 1 )
					{
						*crash_id_a = mask_a[array_n[0]+x];
						*crash_id_b = mask_b[array_n[1]+x];
						return 1;
					}
				x++;
				loop[X]++;
			}
			

			//collision, but not data retrieved, return anwyays
			return 1;
		}

		loop[Y]++;
		y++;
	}


//	return 0;
//*/

	


	loop_no=0;
//	while ( top < height && left<width && loop_no<COLLISION_LOOP_LIMIT) //when they overlap or == we've covered that column/row
	while ( top < height && loop_no<COLLISION_LOOP_LIMIT )
	{ 
		//loop_no++;
/*
		x=left;
		line_check[0] = line_check[1] = 0;
		while(x<8)
		{
			y=top;
			array_n[0] = XY_ARRAY( x + overlap_x_a, y + overlap_y_a, width_a );
			line_check[0] |= (mask_a[ array_n[0] ]>0?1:0) << x;
			array_n[1] = XY_ARRAY( x + overlap_x_b, y + overlap_y_b, width_b );
			line_check[1] |= (mask_b[ array_n[1] ]>0?1:0) << x;
			x++;
		}
	*/


		if ( check_mask_overlap_row( crash_id_a, crash_id_b,  mask_a,	mask_b,	top,	width,left,overlap_x_a,overlap_x_b,overlap_y_a,overlap_y_b,width_a,width_b, u8_mask_a, u8_mask_b) )
			return true;

		if ( top != height-1 )
			if ( check_mask_overlap_row( crash_id_a, crash_id_b,  mask_a,	mask_b,	height-1,	width,left,overlap_x_a,overlap_x_b,overlap_y_a,overlap_y_b,width_a,width_b, u8_mask_a, u8_mask_b ) )
				return true;
			
/*
		y=top;
		array_n[0] = XY_ARRAY( left + overlap_x_a, y + overlap_y_a, width_a );
		array_n[1] = XY_ARRAY( left + overlap_x_b, y + overlap_y_b, width_b );
		line_check[0] = FILL_U8( mask_a[array_n[0]+0] , mask_a[array_n[0]+1 ], mask_a[array_n[0]+2 ], mask_a[array_n[0]+3], mask_a[array_n[0]+4], mask_a[array_n[0]+5], mask_a[array_n[0]+6], mask_a[array_n[0]+7] );
		line_check[1] = FILL_U8( mask_b[array_n[1]+0] , mask_b[array_n[1]+1 ], mask_b[array_n[1]+2 ], mask_b[array_n[1]+3], mask_b[array_n[1]+4], mask_b[array_n[1]+5], mask_b[array_n[1]+6], mask_b[array_n[1]+7] );
		line_check[0] &= (1<<width)-1;
		line_check[1] &= (1<<width)-1;

		if (line_check[0] & line_check[1]) //hit on this row
		{
			y=top;
			x=left;
			while(x<width)
			{
				if (CHECK_MASK(x,y))
					return true;
				x++;
			}
			return true;
		}
*/
	/*	line_check[0] = line_check[1] = 0;
		x=left;
		while(x<width)
		{
			y = height-1;
			array_n[0] = XY_ARRAY( x + overlap_x_a, y + overlap_y_a, width_a );
			line_check[0] |= (mask_a[ array_n[0] ]>0?1:0) << x;
			array_n[1] = XY_ARRAY( x + overlap_x_b, y + overlap_y_b, width_b );
			line_check[1] |= (mask_b[ array_n[1] ]>0?1:0) << x;
			x++;
		}
		if (line_check[0] & line_check[1]) //hit on this row
		{
			y = height-1;
			x=left;
			while(x<width)
			{
				if (CHECK_MASK(x,y))
					return true;
				x++;
			}
			//return true;
		}


		while (x<width)
		{
			y = top;
			if (CHECK_MASK(x,y))
				return true;
			y = height-1;
			if (CHECK_MASK(x,y))
				return true;

			x++;
		}
*/
	/*
		y=top;
		while (y<height)
		{
			x = width-1;
			if (CHECK_MASK(x,y))
				return true;
			x = left;
			if (CHECK_MASK(x,y))
				return true;
			y++;
		}
*/

	//	left++;
		top++;
	//	width--;
		height--;
	}


//*/

	return 0; // didnt collide
}			


