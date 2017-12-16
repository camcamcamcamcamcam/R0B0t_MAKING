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
	int angle = 40;
	int distance = 10000;
	int securityDistance = 150;
	int amplitudeSweep = 50;
	
	while(1){
		printf("X=%d\n",X);
		printf("Y=%d\n",Y);
		printf("TETA=%d\n",TETA);
		go_to_distance_sweep(speed, distance, securityDistance, amplitudeSweep);
		printf("X=%d\n",X);
		printf("Y=%d\n",Y);
		printf("TETA=%d\n",TETA);
		rotate_to_angle(speed, angle);
	}
    return 1;
}
