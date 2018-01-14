/*
     ____    __   ___   ___  _____  ____
    | |__) //\ \ | |_) | |_)  | |  | |_
    |_| \  \\_\/ |_|_) |_|_) _|_|_ |_|__

 *//**
 *  \file  sensor_compass.c
 *  \brief  Concerning the compass sensor.
 */

#include <string.h>
#include <stdio.h>
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

uint8_t sn_compass;
int init_compass;

/*
@desc :
	* the function gets the value of the compass
@param : /
@author : Camille Morin
@return : int value of the compass
*/
int getCompassAngle(){
        int val;
        if ( !get_sensor_value( 0, sn_compass, &val ) ) {
            val = 0;
        }
        return ((val-init_compass)%360);
}

/*
@desc :
	* the function initializes the compass
@param : /
@author : Camille Morin
@return : void
*/
void initCompass(){

	      ev3_sensor_init();
        if ( ev3_search_sensor( HT_NXT_COMPASS, &sn_compass, 0 )) {
                printf( "COMPASS sensor is found\n" );
                get_sensor_value(0,sn_compass,&init_compass);
                printf("COMPASS test_init %d \n",init_compass);
        } else {
                printf( "COMPASS sensor is NOT found\n" );
        }
}
