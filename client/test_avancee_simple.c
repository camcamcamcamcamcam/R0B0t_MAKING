#include <pthread.h>
#include <signal.h>
#include "servo_sonar.h"
#include "motors_wheels.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"


char main (void) {
/*
 * The robot is doing a simple algorithm "snail".
 */
    initMotorServo();
    initSensorSonar();
    initMotorWheels();
	servo_arm_up();
    //while the server did not send the START_MESSAGE, the robot will wait in init_client()
    //initClient(); // will STOP the program if the server is not lauched !

	int speed = getTachoMaxSpeed()/5;
	int angle = 90;
	int distanceLng = 1000;
	int securityDistance = 100;
	int amplitudeSweep = 50;
	
	go_to_distance_sweep_regular_braking(speed, distanceLng, securityDistance, amplitudeSweep);
    return 1;
}
