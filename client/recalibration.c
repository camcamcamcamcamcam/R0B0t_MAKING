#include "servo_sonar.h"
#include "motors_wheels.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"

char recalibration(void){
	int speed = getTachoMaxSpeed()/5;
	int angle = 5;
	int dist_zero;
	int dist_a;

	dist_zero = getDistance(0);
	dist_a = getDistance(-a);
	absolute_servo_sonar(a);
	
	int teta;
	teta = (dist_a * a)/(dist_a + dist_zero);
	
	smallRotation(speed,teta);
	return teta;
}

//test
char main(void){
	initMotorServo();
    initSensorSonar();
    initMotorWheels();
	servo_arm_up();
	
	go_to_distance_no_sweep(speed,500,100);
	int correction = recalibration();
	printf("Recalibration of %d ° \n",teta)
	return 1; 
}

