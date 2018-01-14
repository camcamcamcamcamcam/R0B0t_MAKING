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
@desc : the function enables the robot to do a rotation of a certain angle at a certain speed
@param : 
	* int speed : speed of the tacho used for the rotation
	* int angle : angle of the rotation
@author : Samuel Pierre 
@return : void
*/
void rotate_to_angle(int speed, int angle){

	preciseRotation(speed, angle);
	clearBuffer();

}

/*
@desc : the function enables the robot detect the closest obstacle over an amplitudeAngle specified and to rotate in direction of it
@param : 
	* int speed : speed of the tacho used for the rotation
	* int amplitudeAngle : amplitude of the search
	* int precisionAngle : step of the search
@author : 
@return : (int) return the distance of the obstacle detected.
*/
int rotate_to_closest_obstacle(int speed, int amplitudeAngle, int precisionAngle){

	int i = -amplitudeAngle;
	rotate_to_angle(speed,i);
	int minAngle = i;
	int minDistance = getDistance(0);
	//printf("i=%d\n",i);
	//printf("distance=%d\n",getDistance(0));
	while(i<amplitudeAngle){
		i = i + precisionAngle;
		rotate_to_angle(speed,precisionAngle);
		//printf("i=%d\n",i);
		//printf("distance=%d\n",getDistance(0));
		if(getDistance(0)<minDistance){ // compute again the minimum
			minDistance = getDistance(0);
			minAngle = i;
		}
		Sleep(200);
	}
	//printf("1st phase finished");
	// pour que ?a marche on a rajouter une precision d'angle
	rotate_to_angle(speed,-amplitudeAngle+minAngle+precisionAngle);
	//printf("minAngle = %d",minAngle);

	return minDistance;

}
