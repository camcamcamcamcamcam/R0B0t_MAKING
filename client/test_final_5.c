 #include <pthread.h>
#include <signal.h>
#include "servo_sonar.h"
#include "motors_wheels.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"

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
