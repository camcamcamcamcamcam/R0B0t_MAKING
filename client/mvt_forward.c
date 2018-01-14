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


char go_to_distance_sweep_regular_braking_new_v2(int speed, int distance, int securityDistance,int amplitudeSweep){

	initPosition();
	int distance_sonar = 0;

	// variable used for linear braking
	int speedDivider = 2;
	int brakingDistance = 300;

	char distanceMaxDone = 1;
	char distanceMaxDoneLocal = 1;
	distance_sonar = getMinDistance(60,15);
	printf("Distance sonar : %d\n", distance_sonar);
	//printf("Distance security : %d\n", securityDistance);
	if(distance_sonar>securityDistance){
		goStraight_NonBlocking(speed, distance);
	}
	Sleep(50);
	//printf("Before the loop \n");
	distance_sonar = getDistance_weighted(0);
	int minBuffer = getMinBufferSonar();
	thread_sweep();
	Sleep(50);
	if (minBuffer <= securityDistance) {
		distanceMaxDone = 0;
	}
	while(minBuffer>securityDistance && robot_is_moving()){
		if(minBuffer<brakingDistance){
			// linear braking from speed to speed/5. The speed begins to decrease when reaching 40cm distance from the obstacle.
			distanceMaxDoneLocal = manage_speed(speed,minBuffer-securityDistance,securityDistance,brakingDistance, speedDivider);
			if(distanceMaxDone==1){
				distanceMaxDone = distanceMaxDoneLocal;
			}
		}
		refreshGlobalPosition();
		distance_sonar = getDistance_current_weighted();
		//printf("Distance sonar : %d \n",distance_sonar);
		minBuffer = getMinBufferSonar();
		Sleep(50);
	}
	stop_sweep();
	stopMotors();
	clearBuffer();
	distance_sonar = getDistance_weighted(0);
	return distanceMaxDone;
}
