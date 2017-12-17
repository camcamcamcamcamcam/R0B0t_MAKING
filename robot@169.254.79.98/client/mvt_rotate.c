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
#include "servo_sonar.h"
#include "mvt_forward.h"

#ifndef PI
#define PI 3.14159265
#endif

// Max speed of the tacho motor of the ev3 dev
#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

void rotate_to_angle(int speed, int angle){

	preciseRotation(speed, angle);

}

int rotate_to_closest_obstacle(int speed, int amplitudeAngle, int precisionAngle){

	int i = -amplitudeAngle;
	rotate_to_angle(speed,i);
	int minAngle = i;
	int minDistance = getDistance(0);
	printf("i=%d\n",i);
	printf("distance=%d\n",getDistance(0));
	while(i<amplitudeAngle){
		i = i + precisionAngle;
		preciseRotation(speed,precisionAngle);
		printf("i=%d\n",i);
		printf("distance=%d\n",getDistance(0));
		if(getDistance(0)<minDistance){
			minDistance = getDistance(0);
			minAngle = i;
		}
		Sleep(2000);
	}
	printf("1st phase finished");
	rotate_to_angle(speed,-amplitudeAngle+minAngle+precisionAngle);

	return minDistance;

}
