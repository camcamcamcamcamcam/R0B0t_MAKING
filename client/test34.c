#include <motors_wheels.h>
#include <motors_servo.h>
#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_servo.h"
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

void relacherObjet(){
/*Fonction pour relacher un objet*/
	servo_arm_up();
}

void prendreObjet(){
/*Fonction pour prendre un objet mobile*/
	preciseRotation(180,MAX_SPEED/12);
	servo_arm_down();
	preciseRotation(180,MAX_SPEED/12);
}

int main(){
	initMotorsWheels();
	initMotorsServo();
	relacherObjet();
	prendreObjet();
	goStraight(MAX_SPEED/12,100);
	return 0;
}
