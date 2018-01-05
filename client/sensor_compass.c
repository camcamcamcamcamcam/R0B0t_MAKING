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

int getCompassAngle(){
        int val;
        if ( !get_sensor_value( 0, sn_compass, &val ) ) {
            val = 0;
        }
        return (val);
}


void initCompass(){
        int test_init;
	      ev3_sensor_init();
        if ( ev3_search_sensor( HT_NXT_COMPASS_SENSOR, &sn_compass, 0 )) {
                printf( "COMPASS sensor is found\n" );
                get_sensor_value(0,sn_gyro,&test_init);
                printf("COMPASS test_init %d \n",test_init);
        } else {
                printf( "COMPASS sensor is NOT found\n" );
        }
}
