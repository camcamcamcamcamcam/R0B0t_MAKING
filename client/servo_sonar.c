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

int getDistance(int angle){
	
	int distance = 0;

	if(angle>60 || angle<-60){
		printf("Angle not adapted to the configuration : should be between -50° and 50°\n");
	}
	else{
		absolute_servo_sonar(angle);
		distance = get_sonar_distance();
		bufferSonar[indexSonar] = (int) distance;
		indexSonar = (indexSonar+1)%bufferSonarSize;
	}
	
	return distance;
	
}

int getDistance_weighted(int angle){
	// the function returns the projection of the distance detected over the current axis (angle = 0) of the robot
	// if the distance detected will not disturb the robot (when it moves forward), the distance that is detected is 1000.
	int distance = 0;
	int horiz_distance;
	
	if(angle>60 || angle<-60){
		printf("Angle not adapted to the configuration : should be between -50° and 50°\n");
	}
	else{
		absolute_servo_sonar(angle);
		distance = get_sonar_distance();
		horiz_distance = distance*sin(angle*PI/180);
		if(horiz_distance>DIAMETRE_ROBOT/2){
			distance = 1000;
		}
		else{
			distance = distance*cos(angle*PI/180);
		}
		bufferSonar[indexSonar] = (int) distance;
		indexSonar = (indexSonar+1)%bufferSonarSize;
	}
	
	return distance;
}

int getDistance_current_weighted(){
	// the function returns the projection of the distance detected over the current axis (angle = 0) of the robot
	// if the distance detected will not disturb the robot (when it moves forward), the distance that is detected is 1000.
	int distance = 0;
	int horiz_distance;
	
	int angle = get_absolute_angle_servo();
	distance = get_sonar_distance();
	horiz_distance = distance*sin(angle*PI/180);
	if(horiz_distance>DIAMETRE_ROBOT/2){
		distance = 1000;
	}
	else{
		distance = distance*cos(angle*PI/180);
	}
	bufferSonar[indexSonar] = (int) distance;
	indexSonar = (indexSonar+1)%bufferSonarSize;
	
	return distance;
}

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

void clearBuffer(){
	int i=0;
	while(i<bufferSonarSize){
		bufferSonar[i] = 1000;
		i = i+1;
	}
}

int getMinDistance(int amplitudeAngle, int precisionAngle){
	// get the minimum distance detected by the robot from -amplitudeAngle to amplitudeAngle with precisionAngle as step. 
	int angle = -amplitudeAngle;
	int minDistance;
	//int minAngle;
	minDistance = getDistance_weighted(angle);
	while(angle<amplitudeAngle){
		angle = angle+precisionAngle;
		if (getDistance_weighted(angle)<minDistance){
			minDistance = getDistance_weighted(angle);
		}
		Sleep(10);
		//printf("Distance sonar : %d \n",distance_sonar);
	}
	getDistance_weighted(0);
	return minDistance;
}


/*
int main( void ){
//to test each function, we need the main

	int distance;
	int i;
	char answer;
	
    initMotorServo();
    initSensorSonar();
	
	servo_arm_up();
	
	printf("Initializing..");
	
	answer='y';
	
	while(answer!='n'){
		
		i=-30;
		while (i>=-30 && i<=30){
			printf("In loop \n");
			distance = get_distance(i);
			printf("Distance : %d mm\n", distance);
			i=i+5;
		}
		printf("Continue the test ? (y/n)\n");
		Sleep(100);
		scanf(" %c",&answer);
		
	}
	
	servo_sonar(0);

}
*/