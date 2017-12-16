#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_servo.h"
#include "ev3_sensor.h"
#include "color_sensor.h"
#include "motors_servo.h"
#include "motors_wheels.h"
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
/*Fonction pour relacher un objet
 A faire : enregistrer la position de l'objet lâché sur la carte.*/
	servo_arm_up();
}

void prendreObjet(){
/*Fonction pour prendre un objet mobile*/
	preciseRotation(180,MAX_SPEED/12);
	servo_arm_down();
	preciseRotation(-180,MAX_SPEED/12);
}

int isMovableObstacle(){
/*Fonction pour savoir si l'object devant est un obstacle mobile ou non
 A faire : enregistrer la position de l'objet si ce n'est pas un objet mobile*/
	int a;
	a=redObstacle();
	return(a);
}

int main(){
	initMotorsWheels();
	initMotorsServo();
	relacherObjet();
	if (isMovableObstacle()){
		prendreObjet();
	}
	else {
		preciseRotation(90,MAX_SPEED/12);
	}
	goStraight(MAX_SPEED/12,100);
	return 0;
}
