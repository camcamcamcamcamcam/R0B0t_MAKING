#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"


// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

int value_r;
int value_g;
int value_b;

uint8_t sn_color;

/*
@desc : read the data from the color sensor and update the 3 global variables value_r, value_g, value_b (values RGB).
@param : /
@author : 
@return : void
*/
void getColor(){
    if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
        printf( "COLOR sensor is found, reading COLOR...\n" );
        set_sensor_mode_inx(sn_color,LEGO_EV3_COLOR_RGB_RAW);
        get_sensor_value( 0, sn_color, &value_r );
		get_sensor_value( 1, sn_color, &value_g);
		get_sensor_value( 2, sn_color, &value_b);
        fflush( stdout );
    }
}

/*
@desc : returns an integer (boolean) telling if the obstacle is red or not.
@param : /
@author : 
@return : integer (1 if the obstacle is red, 0 else)
*/
int red_obstacle(){
    getColor();
    if (value_r > value_g + value_b) printf("obstacle red found\n");
    else printf("not red\n");
	printf("returned values are %d %d %d\n",value_r,value_g,value_b);
	return (value_r > value_g + value_b);
}
