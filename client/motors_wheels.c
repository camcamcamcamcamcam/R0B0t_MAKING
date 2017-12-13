#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
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

#ifndef PI
#define PI 3.14159265
#endif

// Max speed of the tacho motor of the ev3 dev
#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

#define DIAMETRE 56  // diameter of the wheel : 56mm
#define DIAMETRE_ROBOT 170 // width of the robot in mm

// Global variables
uint8_t sn_wheels[2];
uint8_t sn_gyro;

// Define the variable used to have the precise absolute position of the robot
int X; // position x en mm.
int Y; // position y en mm.
int TETA; // angle absolu du robot en degré


int get_left_motor_position(){
    /*
	The function enables to get the position of the left motor
	*/
	
    int position;
    get_tacho_position(sn_wheels[1],&position);
    return position;
}

int get_right_motor_position(){
    /*
	The function enables to get the position of the right motor
	*/
	
    int position;
    get_tacho_position(sn_wheels[0],&position);
    return position;
}

void initMotorWheels(){
	/*
	Initializing 2 motors_wheels :
	*/

    if ( ev3_init() == -1 ) return ( 1 );
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );//do not remove this line, or the LEGO_EV3_M_MOTOR 1 will NOT be found
    ev3_search_tacho_plugged_in(66, 0, &sn_wheels[0], 0);
    ev3_search_tacho_plugged_in(68, 0, &sn_wheels[1], 0);

}

int robot_is_moving(){
	/*
	The function returns 0 if the robot is not moving and 1 else.
	*/
	
	int speed;
	int speed2;
	get_tacho_speed( sn_wheels[0], &speed);
	get_tacho_speed( sn_wheels[1], &speed2);
	Sleep(10);
	if(speed == 0 && speed2 == 0){
		return 0;
	}
	else{
		return 1;
	}
	
}

void goStraight(int speed, int distance){
	/*
	The function enables to ask the robot to go straight for a certain distance (in mm) at a specified speed.
	It is a blocking function : the thread including this function won't continue before the robot has reached the desired position.
	*/
    int angle = distance_to_angle(distance);
    synchronisedGoStraight(sn_wheels, speed, angle);
	while(robot_is_moving()){ // waiting until the speed of the two motors has reached 0.
		Sleep(10);
	}
	
}

void rotation(int speed, int angle){
    /*
	The function enables to ask the robot to do a rotation of a certain angle (in°) at a specified speed.
	It is a blocking function : the thread including this function won't continue before the robot has reached the desired position.
	*/
    int distance_roue = (angle * PI * DIAMETRE_ROBOT) / (360);
    int angle_roue = distance_to_angle(distance_roue);
	
    goStraightForAngle(sn_wheels[0], speed, angle_roue);
    goStraightForAngle(sn_wheels[1], speed, -angle_roue);
	while(robot_is_moving()){ // waiting until the speed of the two motors has reached 0.
		Sleep(10);
	}
	
}

void synchronisedGoStraight(uint8_t *sn_wheels, int speed, int angle) {
    /*
	The function enables to ask the robot to run the two motors at the same time until they both reached a certain angle.
	It is a non self-blocking function.
	*/
    if (angle < 0){
        multi_set_tacho_polarity_inx(sn_wheels,TACHO_INVERSED);
        angle = -angle;
    } else{
        multi_set_tacho_polarity_inx(sn_wheels,TACHO_NORMAL);
    }
    multi_set_tacho_speed_sp(sn_wheels, speed);
    multi_set_tacho_position_sp(sn_wheels, angle);
    multi_set_tacho_command_inx(sn_wheels, TACHO_HOLD);
}

void stopMotors(){
	/*
	The function enables to astop the motors. It could be useful in cas of interrupts for example.
	*/
    multi_set_tacho_position_sp(sn_wheels, 0);
    multi_set_tacho_command_inx(sn_wheels, TACHO_HOLD);
}


void goStraightForAngle(uint8_t sn_wheels, int speed, int angle) {
	/*
	The function enables to ask a motor to run to a certain angle at a certain speed.
	*/
    if (angle < 0){
        set_tacho_polarity_inx(sn_wheels,TACHO_INVERSED);
		angle = -angle;
    } else{
        set_tacho_polarity_inx(sn_wheels,TACHO_NORMAL);
    }
    set_tacho_speed_sp(sn_wheels, speed);
    set_tacho_position_sp(sn_wheels, angle);
    set_tacho_stop_action_inx(sn_wheels, TACHO_STOP_ACTION__NONE_);
    set_tacho_command_inx(sn_wheels, TACHO_HOLD);
}

int angle_to_distance(int angle){
	/* 
	The function enables to compute the distance in mm corresponding to a certain angle (°) for the motor.
	*/
    return ((angle/360)*PI*DIAMETRE);
}


int distance_to_angle(int distance){
	/* 
	The function enables to compute the angle in ° corresponding to a certain distance (in mm) for the motor.
	*/
    return ((distance*360)/(PI*DIAMETRE));
}

void initGyro(){
	ev3_sensor_init();
        if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyro, 0 )) {
                printf( "GYRO sensor is found\n" );
                set_sensor_mode( sn_gyro, "GYRO-ANG" );
                set_sensor_mode( sn_gyro, "GYRO-G&A" );
                int test_init;
                get_sensor_value(0,sn_gyro,&test_init);
                printf("test_init %d \n",test_init);
        } else {
                printf( "GYRO sensor is NOT found\n" );
        }
}

int getAngleGyro(){
        int val;
	get_sensor_value(0,sn_gyro,&val);
	return val;
}

void preciseRotation(int angle,int max_speed){
    int angle_gyro;
    int difference;
    int compteur;

    rotation(max_speed / 12, angle);
    initGyro();
    angle_gyro = getAngleGyro();
    difference = angle - angle_gyro;
    printf("angle_gyro = %d, difference = %d\n",angle_gyro,difference);
    compteur = 0;
    while(difference!=0 && compteur <100){
        if (difference <= 180 && difference >= -180) rotation(max_speed / 12, difference);
        Sleep(1000);
        angle_gyro = getAngleGyro();
        difference = angle - angle_gyro;
        printf("difference = %d, angle_gyro = %d, compteur = %d \n",difference,angle_gyro,compteur);
        compteur++;
    }
}

int getTachoMaxSpeed(){
    int max_speed;
    get_tacho_max_speed( sn_wheels[0], &max_speed );
    return max_speed;
}