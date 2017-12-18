/*
     ____    __   ___   ___  _____  ____
    | |__) //\ \ | |_) | |_)  | |  | |_
    |_| \  \\_\/ |_|_) |_|_) _|_|_ |_|__

 *//**
 *  \file  sensor_gyro.c
 *  \brief  Concerning the gyro sensor.
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

uint8_t sn_gyro;

int getGyroAngle(){
        int val;
        if ( !get_sensor_value( 0, sn_gyro, &val ) ) {
            val = 0;
        }
        return (val);
}

int getGyroRotationnalSpeed(){
        int val;
        if ( !get_sensor_value( 1, sn_gyro, &val ) ) {
            val = 0;
        }
        return (val);
}

void initGyro(){
        int test_init;
	      ev3_sensor_init();
        if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyro, 0 )) {
                printf( "GYRO sensor is found\n" );
                set_sensor_mode( sn_gyro, "GYRO-ANG" );
                set_sensor_mode( sn_gyro, "GYRO-G&A" );
                Sleep(100);
                get_sensor_value(0,sn_gyro,&test_init);
                Sleep(100);
                printf("test_init %d \n",test_init);
        } else {
                printf( "GYRO sensor is NOT found\n" );
        }
}