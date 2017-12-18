/*
     ____    __   ___   ___  _____  ____
    | |__) //\ \ | |_) | |_)  | |  | |_
    |_| \  \\_\/ |_|_) |_|_) _|_|_ |_|__

 *//**
 *  \file  test_scan_attrape.c
 *  \brief  Exemple de Robbie qui attrape un objet.
 */

#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_servo.h"
#include "ev3_sensor.h"
#include "color_sensor.h"
#include "motors_servo.h"
#include "motors_wheels.h"
#include "object_interaction.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif
#define MAX_SPEED 1050

int main(){
	initMotorWheels();
	initMotorServo();
	go_to_distance_sweep(MAX_SPEED/6,300,50,40);
	Sleep(2000);
	dropObject();
	Sleep(200);
	go_to_distance_sweep(MAX_SPEED/6,300,50,40);
	return 0;
}
