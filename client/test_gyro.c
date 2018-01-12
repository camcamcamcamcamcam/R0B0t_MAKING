/*
     ____    __   ___   ___  _____  ____
    | |__) //\ \ | |_) | |_)  | |  | |_
    |_| \  \\_\/ |_|_) |_|_) _|_|_ |_|__

 *//**
 *  \file  test_gyro.c
 *  \brief  Concerning the gyro sensor.
 */

#include <string.h>
#include <stdio.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"
#include "sensor_gyro.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif


int main()
{
	initGyro();
	while(1){
		printf("gyro_angle %d\n",getGyroAngle());
		printf("                       gyro_speed % d\n",getGyroRotationnalSpeed());
		Sleep(1000);
	}
	return ( 0 );
}
