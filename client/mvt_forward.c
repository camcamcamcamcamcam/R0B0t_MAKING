#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include <math.h>
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

#include "motors_wheels.h"
#include "motors_servo.h"
#include "sensor_sonar.h"
#include "mvt_forward.h"
#include "servo_sonar.h"

#ifndef PI
#define PI 3.14159265
#endif

// Max speed of the tacho motor of the ev3 dev
#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

void go_to_distance_no_sweep(int speed, int distance, int securityDistance){

	initPosition();
	int distance_sonar = 0;
	distance_sonar = getDistance(0);
	printf("Distance sonar : %d\n", distance_sonar);
	printf("Distance security : %d\n", securityDistance);
	if(distance_sonar>securityDistance){
		goStraight_NonBlocking(speed, distance);
	}
	Sleep(50);
	printf("Before the loop \n");
	while(distance_sonar>securityDistance && robot_is_moving()){
		printf("X : %d \n",X);
		printf("Y : %d \n",Y);
		printf("TETA : %d \n",TETA);
		refreshGlobalPosition();
		Sleep(10);
		distance_sonar = getDistance(0);
		//printf("Distance sonar : %d \n",distance_sonar);
	}
	stopMotors();
	clearBuffer();
}

void go_to_distance_sweep(int speed, int distance, int securityDistance,int amplitudeSweep){

	initPosition();
	int distance_sonar = 0;
	distance_sonar = getDistance(0);
	printf("Distance sonar : %d\n", distance_sonar);
	//printf("Distance security : %d\n", securityDistance);
	if(distance_sonar>securityDistance){
		goStraight_NonBlocking(speed, distance);
	}
	Sleep(50);
	//printf("Before the loop \n");
	int i = -amplitudeSweep;
	int order = 1;
	while(distance_sonar>securityDistance && robot_is_moving()){
		if(distance_sonar<300){
			slow_down(speed/2);
		}
		printf("X : %d \n",X);
		printf("Y : %d \n",Y);
		printf("TETA : %d \n",TETA);
		refreshGlobalPosition();
		if(i>amplitudeSweep){
			order = -1;
		}
		else if(i<-amplitudeSweep){
			order = 1;
		}
		i = i + 15*order;
		distance_sonar = getDistance(i);
		//printf("Distance sonar : %d \n",distance_sonar);
	}
	stopMotors();
	clearBuffer();
	distance_sonar = getDistance(0);
}

void go_to_distance_sweep_regular_braking(int speed, int distance, int securityDistance,int amplitudeSweep){

	initPosition();
	int distance_sonar = 0;
	
	// variable used for linear braking
	int newSpeed;
	int speedDivider = 5;
	int brakingDistance = 500;
	
	distance_sonar = getDistance(0);
	printf("Distance sonar : %d\n", distance_sonar);
	//printf("Distance security : %d\n", securityDistance);
	if(distance_sonar>securityDistance){
		goStraight_NonBlocking(speed, distance);
	}
	Sleep(50);
	//printf("Before the loop \n");
	int i = -amplitudeSweep;
	int order = 1;
	distance_sonar = getDistance(0);
	int minBuffer = getMinBufferSonar();
	while(minBuffer>securityDistance && robot_is_moving()){
		if(minBuffer<brakingDistance && minBuffer>securityDistance){
			// linear braking from speed to speed/5. The speed begins to decrease when reaching 30cm distance from the obstacle.
			newSpeed = speed - (((speedDivider-1)*speed/speedDivider)*(brakingDistance-minBuffer))/(brakingDistance-securityDistance);
			slow_down(newSpeed);
		}
		printf("X : %d \n",X);
		printf("Y : %d \n",Y);
		printf("TETA : %d \n",TETA);
		refreshGlobalPosition();
		if(i>amplitudeSweep){
			order = -1;
		}
		else if(i<-amplitudeSweep){
			order = 1;
		}
		i = i + 15*order;
		distance_sonar = getDistance(i);
		//printf("Distance sonar : %d \n",distance_sonar);
		minBuffer = getMinBufferSonar();
	}
	stopMotors();
	clearBuffer();
	distance_sonar = getDistance(0);
}
