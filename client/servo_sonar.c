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

#include "motors_servo.h"
#include "sensor_sonar.h"

int getDistance(int angle){
	
	int distance = 0;

	if(angle>=50 || angle<=-50){
		printf("Angle not adapted to the configuration : should be between -50° and 50°\n");
	}
	else{
		absolute_servo_sonar(angle);
		Sleep(100);
		distance = get_sonar_distance();
	}
	
	return distance;
	
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