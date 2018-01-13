#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "sensor_gyro.h"
#include <math.h>
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
#define ECART_ROUES 175 // distance between the wheels, in mm

// Global variables
uint8_t sn_wheels[2];
uint8_t sn_gyro;

// Define the variable used to have the current absolute position of the robot
int X; // position x en mm.
int Y; // position y en mm.
int TETA; // angle absolu du robot en degré

// Define some checkpoints to save the precise position of the robot
int X1; // position x en mm.
int Y1; // position y en mm.
int TETA1; // angle absolu du robot en degré

// define some variables used to manage the evolution of absolute variables X, Y, TETA
int leftStartPosition = 0;
int rightStartPosition = 0;
int rotationPolarity = 0;

// define some variables used to manage the evolution of absolute variables X, Y, TETA
int leftFinalPosition = 0;
int rightFinalPosition = 0;

int get_left_motor_position(){
    /*
	The function enables to get the position of the left motor
	*/

    int position;
    get_tacho_position(sn_wheels[1],&position);
	//printf("position motor left : %d \n",position);
    return position;
}

int get_right_motor_position(){
    /*
	The function enables to get the position of the right motor
	*/

    int position;
    get_tacho_position(sn_wheels[0],&position);
	//printf("position motor left : %d \n",position);
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
    printf("Current value X : \n");
    scanf("%d",&X);
    printf("Current value Y: \n");
    scanf("%d",&Y);
    TETA = 0;
	initPosition();

}

int robot_is_moving(){
	/*
	The function returns 0 if the robot is not moving and 1 else.
	*/

	int speed;
	int speed2;
	get_tacho_speed( sn_wheels[0], &speed);
	get_tacho_speed( sn_wheels[1], &speed2);
	if(speed == 0 && speed2 == 0){
		return 0;
	}
	else{
		return 1;
	}

}

void goStraightForAngle(uint8_t sn_wheels, int speed, int angle) {
	/*
	The function enables to ask a motor to run to a certain angle at a certain speed.
	*/
	int position;
    get_tacho_position(sn_wheels,&position);
    if (angle < 0){
        set_tacho_polarity_inx(sn_wheels,TACHO_INVERSED);
		angle = -angle;
    } else{
        set_tacho_polarity_inx(sn_wheels,TACHO_NORMAL);
    }
	set_tacho_position(sn_wheels,position);
    set_tacho_speed_sp(sn_wheels, speed);
    set_tacho_position_sp(sn_wheels, angle);
    set_tacho_command_inx(sn_wheels, TACHO_HOLD);
}

void synchronisedGoStraight(uint8_t *sn_wheels, int speed, int angle) {
    /*
	The function enables to ask the robot to run the two motors at the same time until they both reached a certain angle.
	It is a non self-blocking function.
	*/

	int position_left = get_left_motor_position();
	int position_right = get_right_motor_position();
    if (angle < 0){
        multi_set_tacho_polarity_inx(sn_wheels,TACHO_INVERSED);
		angle = -angle;
    } else{
        multi_set_tacho_polarity_inx(sn_wheels,TACHO_NORMAL);
    }
	set_tacho_position(sn_wheels[1],position_left);
	set_tacho_position(sn_wheels[0],position_right);
    multi_set_tacho_speed_sp(sn_wheels, speed);
    multi_set_tacho_position_sp(sn_wheels, angle);
	leftFinalPosition = position_left + angle;
	rightFinalPosition = position_right + angle;
	//multi_set_tacho_stop_action_inx(sn_wheels, TACHO_STOP_ACTION__NONE_);
    multi_set_tacho_command_inx(sn_wheels, TACHO_HOLD);
}

void goStraight(int speed, int distance){
	/*
	The function enables to ask the robot to go straight for a certain distance (in mm) at a specified speed.
	It is a blocking function : the thread including this function won't continue before the robot has reached the desired position.
	*/
	initPosition();

    int angle = distance_to_angle(distance);
    synchronisedGoStraight(sn_wheels, speed, angle);
	Sleep(10);
	while(robot_is_moving()){ // waiting until the speed of the two motors has reached 0.
		//printf("Robot moving\n");
		Sleep(10);
		refreshGlobalPosition();
	}

	refreshPosition();

}

void slow_down(int speed){
	multi_set_tacho_speed_sp(sn_wheels, speed);
    multi_set_tacho_command_inx(sn_wheels, TACHO_HOLD); // Has to be replace !!
}


char manage_speed(int max_speed, int maxDistance,int securityDistance,int brakingDistance, int speedDivider){
	int distance;
	int angle;
	int deltaAngle = fabs(leftFinalPosition-leftStartPosition)-fabs(get_left_motor_position()-leftStartPosition);
	char distanceMaxDone = 1;
	if(deltaAngle<0){
		distance=0;
		deltaAngle=0;
	}
	else{
		distance = angle_to_distance(deltaAngle);
	}
	printf("Distance remaining : %d \n",distance);
	printf("Maximum distance that the robot should do : %d \n",maxDistance);
	if(distance>maxDistance){
		distance = maxDistance;
		deltaAngle = distance_to_angle(distance);
		leftFinalPosition = get_left_motor_position() + deltaAngle;
		rightFinalPosition = get_right_motor_position() + deltaAngle;
		distanceMaxDone = 0
	}
	int newSpeed = max_speed - (((speedDivider-1)*max_speed/speedDivider)*(brakingDistance-distance))/(brakingDistance-securityDistance);
	multi_set_tacho_speed_sp(sn_wheels, newSpeed);
	multi_set_tacho_position_sp(sn_wheels, deltaAngle);
    multi_set_tacho_command_inx(sn_wheels, TACHO_HOLD); // Has to be replace !!
	return distanceMaxDone;
}

void goStraight_NonBlocking(int speed, int distance){
	/*
	The function enables to ask the robot to go straight for a certain distance (in mm) at a specified speed.
	It is a non blocking function : the thread including this function will continue.
	*/
    int angle = distance_to_angle(distance);
    //goStraightForAngle(sn_wheels[0], speed, angle);
	//goStraightForAngle(sn_wheels[1], speed, angle);
	synchronisedGoStraight(sn_wheels, speed, angle);

}

void rotation(int speed, int angle){
    /*
	The function enables to ask the robot to do a rotation of a certain angle (in°) at a specified speed.
	It is a blocking function : the thread including this function won't continue before the robot has reached the desired position.
	*/

	if(angle<0){
		rotationPolarity = -1;
	}
	else{
		rotationPolarity = 1;
	}

    int distance_roue = (angle * PI * ECART_ROUES) / (360);
    int angle_roue = distance_to_angle(distance_roue);
    goStraightForAngle(sn_wheels[0], speed, -angle_roue);
    goStraightForAngle(sn_wheels[1], speed, angle_roue);
	Sleep(10);
	while(robot_is_moving()){ // waiting until the speed of the two motors has reached 0.
		Sleep(10);
	}


}

void smallRotation(int speed, int angle){
	/*
	The function enables to ask the robot to do a precise rotation of a certain angle (in°) at a specified speed, using the gyroscope.
	It is a blocking function : the thread including this function won't continue before the robot has reached the desired position.
	*/

    int difference;
	initPosition();
    rotation(speed, angle);
	TETA1 = (TETA1 + angle)%360;
	TETA = TETA1;
	//printf("TETA=%d \n",TETA);
	initPosition();

}

void preciseRotation(int speed, int angle){
	/*
	The function enables to ask the robot to do a precise rotation of a certain angle (in°) at a specified speed, using the gyroscope.
	It is a blocking function : the thread including this function won't continue before the robot has reached the desired position.
	*/

  int angle_gyro_start;
  int angle_gyro_end;
  int difference;
	initPosition();
	//printf("commencer angle : %d \n",angle);
	angle_gyro_start = getGyroAngle();
	//printf("angle gyro avant : %d \n",(int) getAngleGyro());
  rotation(speed, angle);
	//printf("angle gyro apres : %d \n",(int) getAngleGyro());
	angle_gyro_end = getGyroAngle();
	difference = angle - (angle_gyro_end - angle_gyro_start);
	//printf("angle restant : %d \n",difference);
	if(difference!=0){
		rotation(speed, difference);
	}
	TETA1 = (TETA1 + angle)%360;
	TETA = TETA1;
	//printf("TETA=%d \n",TETA);
	initPosition();

}

void initPosition(){
	/*
	Save the configuration of the robot in the variables leftStartPosition, rightStartPosition
	*/
	leftStartPosition = get_left_motor_position();
	rightStartPosition = get_right_motor_position();
	//printf("Left start pos : %d\n", leftStartPosition);
	//printf("Right start pos : %d\n", rightStartPosition);
	rotationPolarity = 0;
	X1 = X;
	Y1 = Y;
}

void refreshPosition(){
	/*
	refresh the position X,Y and the angle TETA of the robot.
	*/

	int difference;
	int meanAngle;

	if(rotationPolarity==0){

		//printf("Left motor pos=%d\n", (int) get_left_motor_position());
		//printf("Right motor pos=%d\n", (int) get_left_motor_position());
		meanAngle = (fabs(get_left_motor_position()-leftStartPosition)+fabs(get_right_motor_position()-rightStartPosition))/2;
		difference = angle_to_distance(meanAngle);
		X1 = X1 + difference*sin(TETA*PI/180);
		Y1 = Y1 + difference*cos(TETA*PI/180);
		X=X1;
		Y=Y1;
		initPosition();

	}

}

void refreshGlobalPosition(){
	/*
	refresh the position X,Y and the angle TETA of the robot.
	*/

	int difference;
	int meanAngle;

	if(rotationPolarity==0){
		//printf("RotationPolarity=0\n");
		//printf("Left motor pos=%d\n", (int) get_left_motor_position());
		//printf("Right motor pos=%d\n", (int) get_left_motor_position());
		meanAngle = (int) (fabs(get_left_motor_position()-leftStartPosition)+fabs(get_right_motor_position()-rightStartPosition))/2;
		//printf("meanAngle=%d\n", meanAngle);
		difference = angle_to_distance(meanAngle);
		//printf("Difference Distance=%d\n",difference);
		X = X1 + difference*sin(TETA*PI/180);
		Y = Y1 + difference*cos(TETA*PI/180);

	}

}

void stopMotors(){
	/*
	The function enables to astop the motors. It could be useful in cas of interrupts for example.
	*/
    multi_set_tacho_position_sp(sn_wheels, 0);
    multi_set_tacho_command_inx(sn_wheels, TACHO_STOP);
	while(robot_is_moving()){ // waiting until the speed of the two motors has reached 0.
		Sleep(10);
	}
	refreshPosition();
}

int angle_to_distance(int angle){
	/*
	The function enables to compute the distance in mm corresponding to a certain angle (°) for the motor.
	*/
    return (angle*PI*DIAMETRE)/360;
}


int distance_to_angle(int distance){
	/*
	The function enables to compute the angle in ° corresponding to a certain distance (in mm) for the motor.
	*/
    return ((distance*360)/(PI*DIAMETRE));
}


int getTachoMaxSpeed(){
    int max_speed;
    get_tacho_max_speed( sn_wheels[0], &max_speed );
    return max_speed;
}
