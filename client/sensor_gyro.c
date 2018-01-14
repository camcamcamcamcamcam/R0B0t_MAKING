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
int init_gyro;

/*
@desc :
	* the function gets the angle
@param : /
@author : Camille Morin
@return : int val (value of the angle)
*/
int getGyroAngle(){
        int val;
        if ( !get_sensor_value( 0, sn_gyro, &val ) ) {
            val = init_gyro;
        }
        return val;
}

/*
@desc :
	* the function gets the rotationnal speed
@param : /
@author : Camille Morin
@return : int val (value of the rotationnal speed)
*/
int getGyroRotationnalSpeed(){
        int val;
        if ( !get_sensor_value( 1, sn_gyro, &val ) ) {
            val = 0;
        }
        return (val);
}

/*
@desc :
	* the function initializes the gyro with the mode that gives the angle and the rotational speed
@param : /
@author : Camille Morin
@return : void
*/
void initGyro(){
        ev3_sensor_init();
        if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyro, 0 )) {
                printf( "GYRO sensor is found\n" );
                set_sensor_mode( sn_gyro, "GYRO-G&A" );
                Sleep(10);
                get_sensor_value(0,sn_gyro,&init_gyro);
                Sleep(10);
                printf("init_gyro %d \n",init_gyro);
        } else {
                printf( "GYRO sensor is NOT found\n" );
        }
}
