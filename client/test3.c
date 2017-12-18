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
#define MAX_SPEED 1050
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif


int main(){
	initMotorsWheels();
	initMotorsServo();
	go_to_distance_no_sweep(MAX_SPEED/8,500,50);
	if (isMovableObstacle()){
		printf("IS MOVABLE OBJECT");
		takeObject();
	}
	else {
		rotate_to_angle(MAX_SPEED/6,90);
		printf("IS NOT MOVABLE");
	}
	go_to_distance_no_sweep(MAX_SPEED/6, 100, 35);
	return 0;
}
