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
#define RMIN 35 //rayon d'approche maximale de l'objet

int[2] scan(int angle){
	int angle;
	int distMin;
	int distSonar;
	int[2] distMinAndAngle;
	initSensorSonar();
	rotate_to_angle(MAX_SPEED/12,-angle/2);
	Sleep(500);
	rotate_to_angle(MAX_SPEED/24,angle);
	distMin=get_sonar_distance();
	while(!robot_is_moving()){
		distSonar=get_sonar_distance();
		if (distSonar<distmin){
			distmin=getDistance();
			angle=get_sonar_distance();
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
	rotate_to_angle(MAX_SPEED/12,-(tetha-distMinAndAngle[1]));
	while (get_sonar_distance()>RMIN){
		if (get_sonar_distance()<distMinAndAngle[0]){
			go_to_distance_no_sweep(MAX_SPEED/12,RMIN,RMIN);
		}
		else{
			distMinAndAngle = scan(theta);
		}
	}
	if(isMovableObstacle()){
		takeObject();
		go_to_distance_no_sweep(MAX_SPEED,100,RMIN);
	}
	else{
		rotate_to_angle(MAX_SPEED,90);
		go_to_distance_no_sweep(MAX_SPEED,100,RMIN);
	}


}
