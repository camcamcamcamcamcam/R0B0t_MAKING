#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_servo.h"
#include "ev3_sensor.h"
#include "servo_sonar.h"
#include "sensor_sonar.h"
#include "motors_servo.h"
#include "motors_wheels.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"

int recalibration(int speed){
	int a = 5;
	int dist_zero;
	int dist_a;

	dist_zero = getDistance(0);
	dist_a = getDistance(-a);
	getDistance(a);
	
	int teta;
	teta = (dist_a * a)/(dist_a + dist_zero);
	
	smallRotation(speed,teta);
	return teta;
}

//test
int main(void){
	initMotorServo();
    initSensorSonar();
    initMotorWheels();
	
	int speed = getTachoMaxSpeed()/5;
	
	go_to_distance_no_sweep(speed,500,100);
	int teta = recalibration(speed);
	printf("Recalibration of %d ° \n",teta);
	return 1; 
}

