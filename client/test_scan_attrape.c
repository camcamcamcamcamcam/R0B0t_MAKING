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
#include "sensor_sonar.h"
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

#define MAX_SPEED 1050
#define RMIN 35 //rayon d'approche maximale de l'objet

int[2] scan(int angle){
	int angle;
	int distMin;
	int distSonar;
	int[2] distMinAndAngle;
	initSensorSonar();
	rotationPrecise(-angle/2,MAX_SPEED/12);
	Sleep(500);
	rotationPrecise(angle,MAX_SPEED/24);
	distMin=getSonarValue();
	while(!robot_is_moving()){
		distSonar=getSonarValue();
		if (distSonar<distmin){
			distmin=distSonar();
			angle=getGyroAngle();
		}
	}
	distMinAndAngle[0]=distMin;
	distMinAndAngle[1]=angle;
	return distMinAndAngle;
}

int main{
	int theta;
	int[2] distMinAndAngle;
	distMinAndAngle = scan(theta);
	rotationPrecise(-(tetha-distMinAndAngle[1]),MAX_SPEED/12);
	while (getSonarValue()>RMIN){
		if (getSonarValue()<distMinAndAngle[0]){
			goStraight(MAX_SPEED/12,RMIN);
		}
		else{
			distMinAndAngle = scan(theta);
		}
	}
	if(isMovableObstacle()){
		prendreObjet();
		goStraight(MAX_SPEED,100);	
	}
	else{
		preciseRotation(90,MAX_SPEED);
		goStraight(MAX_SPEED,100);
	}


}
