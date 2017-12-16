#include <pthread.h>
#include <signal.h>
#include "servo_sonar.h"
#include "motors_wheels.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"

#ifndef PI
#define PI 3.14159265
#endif

char main (void) {
/*
 * The robot go straight and stop when he detects a wall to close ( < 50 mm )
 */
    initMotorServo();
    initSensorSonar();
    initMotorWheels();
	servo_arm_up();
    //while the server did not send the START_MESSAGE, the robot will wait in init_client()
    // initClient(); // will STOP the program if the server is not lauched !
	int initialX;
	int initialY;
	int initialTETA;
	printf("Current X : \n");
	scanf("%d",&initialX);
	printf("\n");
	printf("Current Y : \n");
	scanf("%d",&initialY);
	printf("\n");
	printf("Current TETA : \n");
	scanf("%d",&initialTETA);
	printf("\n");
	
	int finalX = 0;
	int finalY = 0;
	int finalTETA = 0;
	
	int deltaX = finalX - initialX;
	int deltaY = finalY - initialY;
	
	int firstRotation;
	if(deltaX>0 && deltaY>0){
		firstRotation = (int) 90-(atan(fabs(deltaY/deltaX))*180)/PI;
	}
	else if(deltaX>0 && deltaY<0){
		firstRotation = (int) 90+(atan(fabs(deltaY/deltaX))*180)/PI;
	}
	else if(deltaX<0 && deltaY>0){
		firstRotation = (int) -(90-(atan(fabs(deltaY/deltaX))*180)/PI);
	}
	else{
		firstRotation = (int) -(90+(atan(fabs(deltaY/deltaX))*180)/PI);
	}
	int speed = getTachoMaxSpeed()/5;
	int securityDistance = 150;
	int amplitudeSweep = 50;
	
	rotate_to_angle(speed,firstRotation-initialTETA);
	int distance = (int) sqrt(pow(deltaX,2)+pow(deltaY,2));
	go_to_distance_sweep(speed, distance, securityDistance, amplitudeSweep);
	rotate_to_angle(speed,finalTETA-TETA);
	
    return 1;
}
