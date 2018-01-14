#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include <math.h>
#include "motors_wheels.h"
#include "motors_servo.h"
#include "sensor_sonar.h"
#include "mvt_forward.h"
#include "servo_sonar.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

#ifndef PI
#define PI 3.14159265
#endif

// Max speed of the tacho motor of the ev3 dev
#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

/*
@desc : 
	* the function enables the robot to move forward until the distance specified has been reached or an obstacle has been met
	* before the movement, the robot is doing a scan around it to see if it can move forward
	* during the movement, the sonar is sweeping the space
	* if braking is needed, the robot adapt his speed to the environnement
	* at the end, the function returns a char describing if the robot has done the distance specified (1) or if it was obliged to stop because of obstacles (0)
@param : 
	* int speed : speed of the tacho used for the rotation
	* int distance : maximum distance the robot should cover during the movement
	* int securityDistance : distance of security set to avoid bumping into obstacles
	* amplitudeSweep : amplitude of the sweep of the sonar during the movement
@author : Samuel Pierre
@return : (char) a char describing if the robot has done the distance specified (1) or if it was obliged to stop because of obstacles (0)
*/
char go_to_distance_sweep_regular_braking_new_v2(int speed, int distance, int securityDistance,int amplitudeSweep){

	initPosition();
	int distance_sonar = 0;

	// variable used for linear braking and checking if the robot has completed the movement or not
	int speedDivider = 2;
	int brakingDistance = 300;
	char distanceMaxDone = 1;
	char distanceMaxDoneLocal = 1;
	
	// scanning the environnement before going forward
	distance_sonar = getMinDistance(60,15);
	if(distance_sonar>securityDistance){
		// if the robot is enough far from the obstacle, it moves and adapts directly its speed
		goStraight_NonBlocking(speed, distance);
		if(minBuffer<brakingDistance){
			distanceMaxDoneLocal = manage_speed(speed,distance_sonar-securityDistance,securityDistance,brakingDistance, speedDivider);
			if(distanceMaxDone==1){
				distanceMaxDone = distanceMaxDoneLocal;
			}
		}
	}
	Sleep(50);
	
	// put the head of the robot in front of it
	distance_sonar = getDistance_weighted(0);
	int minBuffer = getMinBufferSonar();
	// beginning of the sweep
	thread_sweep();
	// if the robot has not enough space, we indicate that it won't be able to achieve the movement
	if (minBuffer <= securityDistance) { 
		distanceMaxDone = 0;
	}
	
	while(minBuffer>securityDistance && robot_is_moving()){
		if(minBuffer<brakingDistance){
			// linear braking from speed to speed/speedDivider. The speed begins to decrease when reaching brakingDistance from the obstacle.
			distanceMaxDoneLocal = manage_speed(speed,minBuffer-securityDistance,securityDistance,brakingDistance, speedDivider);
			if(distanceMaxDone==1){
				distanceMaxDone = distanceMaxDoneLocal;
			}
		}
		refreshGlobalPosition();
		// get the distance of the sonar
		distance_sonar = getDistance_current_weighted();
		minBuffer = getMinBufferSonar();
		Sleep(50);
	}
	
	// Stop the motors and clear the buffer of the sonar
	stop_sweep();
	stopMotors();
	clearBuffer();
	
	// restore the state of the head
	distance_sonar = getDistance_weighted(0);
	return distanceMaxDone;
}
