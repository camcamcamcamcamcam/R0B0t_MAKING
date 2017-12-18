#include <pthread.h>
#include <signal.h>
#include "servo_sonar.h"
#include "motors_wheels.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"


void client_position(){
	while(1){

		Sleep(2000);
		int x = (int) (X/50);
		int y = (int) (Y/50);
		//x and y must be between 0 and 80 (ie inside the map)
		x = (x < (MAP_WIDTH-1)) ? ((x >= 0) ? x : 0) : (MAP_WIDTH-1);
		y = (y < (MAP_HEIGHT-1)) ? ((y >= 0) ? y : 0 ) : (MAP_HEIGHT-1);
		sendMessage(MSG_POSITION, x, y, 0, 0, 0, 0);

	}
}


char main (void) {
/*
 * The robot go straight and stop when he detects a wall to close ( < 50 mm )
 */
    initMotorServo();
    initSensorSonar();
    initMotorWheels();
	servo_arm_up();
    //while the server did not send the START_MESSAGE, the robot will wait in init_client()
    initClient(); // will STOP the program if the server is not lauched !

	int speed = getTachoMaxSpeed()/5;
	int angle = 90;
	int distanceShort = 170;
	int distanceLng = 170;
	int securityDistance = 150;
	int amplitudeSweep = 50;

	rotate_to_angle(speed, angle);
	
	while(1){
		printf("X=%d\n",X);
		printf("Y=%d\n",Y);
		go_to_distance_sweep(speed, distanceShort, securityDistance, amplitudeSweep);
		rotate_to_angle(speed, -angle);
		while(getMinDistance(45,15)<securityDistance){
			rotate_to_angle(speed, -angle);
		}
		printf("X=%d\n",X);
		printf("Y=%d\n",Y);
		go_to_distance_sweep(speed, distanceLng, securityDistance, amplitudeSweep);
		rotate_to_angle(speed, -angle);
		while(getMinDistance(45,15)<securityDistance){
			rotate_to_angle(speed, -angle);
		}
		printf("X=%d\n",X);
		printf("Y=%d\n",Y);
		go_to_distance_sweep(speed, distanceLng, securityDistance, amplitudeSweep);
		rotate_to_angle(speed, angle);
		while(getMinDistance(45,15)<securityDistance){
			rotate_to_angle(speed, angle);
		}
		printf("X=%d\n",X);
		printf("Y=%d\n",Y);
		go_to_distance_sweep(speed, distanceShort, securityDistance, amplitudeSweep);
		rotate_to_angle(speed, angle);
		while(getMinDistance(45,15)<securityDistance){
			rotate_to_angle(speed, angle);
		}
		distanceLng = distanceLng +170;
		printf("X=%d\n",X);
		printf("Y=%d\n",Y);
		go_to_distance_sweep(speed, distanceLng, securityDistance, amplitudeSweep);
		rotate_to_angle(speed, angle);
		while(getMinDistance(45,15)<securityDistance){
			rotate_to_angle(speed, angle);
		}
		printf("X=%d\n",X);
		printf("Y=%d\n",Y);
		go_to_distance_sweep(speed, distanceLng, securityDistance, amplitudeSweep);
		rotate_to_angle(speed, -angle);
		while(getMinDistance(45,15)<securityDistance){
			rotate_to_angle(speed, -angle);
		}
		distanceLng = distanceLng +170;
	}
    return 1;
}
