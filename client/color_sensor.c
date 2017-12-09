#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
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

int red_obstacle(){
    getColor();
    if ((value_r > 10) && (value_g < 10) && (value_b < 10)) printf("obstacle red found\n");	/*tests a modifier, mais l'idee est là*/
    else printf("not red\n");
	printf("returned values are %d %d %d\n",value_r,value_g,value_b);
	return (value_r > 0);
}

int main( void )
{

    if ( ev3_init() == -1 ) return ( 1 );

    printf( "*** ( EV3 ) Hello! ***\n" );

//Run all sensors
    ev3_sensor_init();

    while (1){

        red_obstacle();
        //getColor();
        //printf("Value : %d %d %d \n",value[0], value[1], value[2]);
        sleep(2);

    }

    ev3_uninit();
    printf( "*** ( EV3 ) Bye! ***\n" );

    return ( 0 );
}