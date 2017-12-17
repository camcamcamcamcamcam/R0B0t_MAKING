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
#include "motors_servo.h"
#include "motors_wheels.h"
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

#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

void dropObject(){
/*Fonction pour relacher un objet
 A faire : enregistrer la position de l'objet lâché sur la carte.*/
	servo_arm_up();
}

void takeObject(){
/*Fonction pour prendre un objet mobile
 *Makes a uturn take the object make a second uturn*/
 rotate_to_angle(MAX_SPEED/6,180);
 servo_arm_down();
 rotate_to_angle(MAX_SPEED/6,-180);
}

int isMovableObstacle(){
/*Fonction pour savoir si l'object devant est un obstacle mobile ou non
 A faire : enregistrer la position de l'objet si ce n'est pas un objet mobile*/
	int a;
	absolute_servo_sonar(60);
	Sleep(100);
	a=red_obstacle();
	Sleep(100);
	absolute_servo_sonar(-60);
	Sleep(100);
	return(a);
}
