#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
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

#ifndef PI
#define PI 3.14159265
#endif

#ifndef DIAMETRE_ROBOT
#define DIAMETRE_ROBOT 233
#endif

#include "motors_servo.h"
#include "sensor_sonar.h"

int bufferSonarSize = 10;
int bufferSonar[10] = {1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}; // contains the ten last values of the sonar value.
int indexSonar; // index of the next element to write in the buffer

/*
@desc : the function enables to look at a certain angle (thanks to the servo linked to the sonar) and return the distance detcetd by the sonar without any treatment
@param :
	* int angle : angle used by the servo_sonar
@author : Samuel Pierre
@return : distance detected in mm
*/
int getDistance(int angle){

	int distance = 0;

	absolute_servo_sonar(angle);
	distance = get_sonar_distance();
	bufferSonar[indexSonar] = (int) distance;
	indexSonar = (indexSonar+1)%bufferSonarSize;

	return distance;

}

/*
@desc : 
	* the function get the distance the robot should be able to do without bumping in an object
	* it returns the projection of the distance detected over his moving axis
	* it returns 1000 if the obstacle detected will not concern it (not in its trajectory)
@param :
	* int angle : angle used by the servo_sonar
@author : Samuel Pierre
@return : distance detected in mm
*/
int getDistance_weighted(int angle){
	
	int distance = 0;
	int horiz_distance;

	absolute_servo_sonar(angle);
	distance = get_sonar_distance();
	horiz_distance = fabs(distance*sin((angle*PI)/180));
	if(horiz_distance> (DIAMETRE_ROBOT/2+20)){ // 10 is used as an epsilon to ensure that the robot will not bump into the object
		distance = 1000;
	}
	else{
		distance = fabs(distance*cos((angle*PI)/180));
	}
	bufferSonar[indexSonar] = (int) distance;
	indexSonar = (indexSonar+1)%bufferSonarSize;

	return distance;
}

/*
@desc : 
	* the function get the distance the robot should be able to do without bumping in an object
	* it returns the projection of the distance detected over his moving axis
	* it returns 1000 if the obstacle detected will not concern it (not in its trajectory)
	* it does the same thing than getDistance_weighted(int angle) but the angle used is the current one.
@param : /
@author : Samuel Pierre
@return : distance detected in mm
*/
int getDistance_current_weighted(){
	int distance = 0;
	int horiz_distance;

	int angle = get_absolute_angle_servo();
	distance = get_sonar_distance();
	horiz_distance = fabs(distance*sin((angle*PI)/180));
	if(horiz_distance>DIAMETRE_ROBOT/2+20){
		distance = 1000;
	}
	else{
		distance = fabs(distance*cos((angle*PI)/180));
	}
	bufferSonar[indexSonar] = (int) distance;
	indexSonar = (indexSonar+1)%bufferSonarSize;

	return distance;
}

/*
@desc : 
	* the function returns the minimum value of the buffer associated with the sonar.
@param : /
@author : Samuel Pierre
@return : distance detected in mm
*/
int getMinBufferSonar(){
	int i=0;
	int minDistance=bufferSonar[0];
	while(i<bufferSonarSize){
		if(bufferSonar[i]<minDistance){
			minDistance = bufferSonar[i];
		}
		i = i+1;
	}
	return minDistance;
}

/*
@desc : 
	* the function enables to clear the buffer of the sonar, which might be useful after having met an obstacle for example and having taken associated decision
@param : /
@author : Samuel Pierre
@return : distance detected in mm
*/
void clearBuffer(){
	int i=0;
	while(i<bufferSonarSize){
		bufferSonar[i] = 1000;
		i = i+1;
	}
}

/*
@desc : 
	* the function enables to do a quick scanning of the nearby in front of the robot and to give the max distance the robot can hope to do without bumping into an obstacle
@param : 
	* amplitudeAngle : amplitude of the scanf
	* precisionAngle : step of the scan
@author : Samuel Pierre
@return : distance detected in mm
*/
int getMinDistance(int amplitudeAngle, int precisionAngle){
	int angle = -amplitudeAngle;
	int minDistance;
	int distance;
	minDistance = getDistance_weighted(angle);
	while(angle<amplitudeAngle){
		angle = angle+precisionAngle;
		distance = getDistance_weighted(angle);
		if (distance<minDistance){
			minDistance = distance;
		}
	}
	getDistance_weighted(0);
	return minDistance;
}
