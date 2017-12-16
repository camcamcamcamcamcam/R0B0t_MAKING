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

/*gives the angle*/
int getGyroAngle();

/*gives the rotational speed*/
int getGyroRotationnalSpeed();

/*Initializes the gyro sensor, puts angle to zero*/ 
void initGyro();
