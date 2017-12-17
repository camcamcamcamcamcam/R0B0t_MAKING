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

	int speed = getTachoMaxSpeed()/12;
	int angle = 180;
	int minDistance;
	/*
    rotate_to_angle(speed, angle);
	Sleep(3000);
	rotate_to_angle(speed, angle);
	*/
	minDistance = rotate_to_closest_obstacle(speed, 90, 10);
	go_to_distance_no_sweep(getTachoMaxSpeed()/5, minDistance-50, 50);
	minDistance = rotate_to_closest_obstacle(speed, 45, 5);
    return 1;
}
