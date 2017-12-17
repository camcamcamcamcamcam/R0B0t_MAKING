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

/* OBSOLETE
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
*/
int main(){
	int minDistance;
	initMotorServo();
	initSensorSonar();
	initMotorWheels();
	servo_arm_up();
	minDistance = rotate_to_closest_obstacle(MAX_SPEED/12,90,10);
	printf("fin de rotate_to_obstacle 1\n");
	go_to_distance_no_sweep(MAX_SPEED/5, minDistance-50, 50);
	Sleep(50);
	minDistance = rotate_to_closest_obstacle(MAX_SPEED/12, 30, 3);
	printf("fin de rotate_to_obstacle 2\n");
	if(isMovableObstacle()){
		printf("dans la boucle is MovableObstacle\n");
		takeObject();
		go_to_distance_sweep(MAX_SPEED/6,100,RMIN,40);
	}
	else{
		rotate_to_angle(MAX_SPEED/6,90);
		go_to_distance_sweep(MAX_SPEED/6,100,RMIN,40);
	}
}
