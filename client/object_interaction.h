/*
     ____    __   ___   ___  _____  ____
    | |__) //\ \ | |_) | |_)  | |  | |_
    |_| \  \\_\/ |_|_) |_|_) _|_|_ |_|__

 *//**
 *  \file  object_interaction.c
 *  \brief  Functions when close to an object.
 */

#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_servo.h"
#include "ev3_sensor.h"
#include "color_sensor.h"
#include "motors_wheels.h"
#include "motors_servo.h"
#include "mvt_rotate.h"
#include "mvt_forward.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

int isMovableObstacle();
void dropObject();
void takeObject();
