#include <pthread.h>
#include <signal.h>
#include "servo_sonar.h"
#include "motors_wheels.h"
#include "mvt_forward.h"

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
	int distance = 1000;
	int securityDistance = 100;
	int amplitudeSweep = 50;
	
    go_to_distance_no_sweep(speed, distance, securityDistance);
	Sleep(3000);
	go_to_distance_sweep(speed, distance, securityDistance,amplitudeSweep);
    return 1;
}
