#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "sensor_gyro.h"
#include <math.h>
#include "motors_wheels.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

#ifndef PI
#define PI 3.14159265
#endif

// Max speed of the tacho motor of the ev3 dev
#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

#define DIAMETRE 56  // diameter of the wheel : 56mm
#define ECART_ROUES 160 // distance between the wheels, in mm
#define DIAMETRE_ROBOT 232 // width of the robot in mm

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
int turnBack = 0;

// define some variables used to manage the evolution of absolute variables X, Y, TETA
int leftFinalPosition = 0;
int rightFinalPosition = 0;

/*
@desc :
	* the function initializes the motor linked to the wheels
	* the global position variables  X,Y,TETA are initiliazed in this function
@param : /
@author : Samuel Pierre
@return : void
*/
void initMotorWheels(){

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

/*
@desc :
	* the function enables to get the position of the left motor
@param : /
@author : Samuel Pierre
@return : position (number of ticks of the left motor)
*/
int get_left_motor_position(){

    int position;
    get_tacho_position(sn_wheels[1],&position);
    return position;

}

/*
@desc :
	* the function enables to get the position of the right motor
@param : /
@author : Samuel Pierre
@return : position (number of ticks of the right motor)
*/
int get_right_motor_position(){

    int position;
    get_tacho_position(sn_wheels[0],&position);
    return position;
}

/*
@desc :
	* the function describes if the robot is moving or not
@param : /
@author : Samuel Pierre
@return : return an integer (1 if the robot is moving, 0 else)
*/
int robot_is_moving(){

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

/*
@desc :
	* the function enables to ask a motor to run to a certain angle at a certain speed.
@param :
	* uint8_t sn_wheels : pointer of the motor concerned by this order
	* int speed : speed of the motor
	* int angle : angle the motor has to do
@author : Benjamin Castellan and Samuel Pierre
@return : void
*/
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

/*
@desc :
	* the function enables to ask the motors to run to a certain angle with a certain speed.
	* leftFinalPosition and rightFinalPosition are useful to know the remaining distance the robot has to cover.
	* the function is NON-BLOCKING : the current thread will continue even if the motors have not done the specified distance.
@param :
	* uint8_t *sn_wheels : set of pointers of the two motors linked with the wheels
	* int speed : speed of the motor
	* int angle : angle the motor has to do
@author : Benjamin Castellan and Samuel Pierre
@return : void
*/
void synchronisedGoStraight(uint8_t *sn_wheels, int speed, int angle) {

	int position_left = get_left_motor_position();
	int position_right = get_right_motor_position();
    if (angle < 0){
        multi_set_tacho_polarity_inx(sn_wheels,TACHO_INVERSED);
		angle = -angle;
		turnBack = 1;
    } else{
        multi_set_tacho_polarity_inx(sn_wheels,TACHO_NORMAL);
		turnBack = 0;
    }
	set_tacho_position(sn_wheels[1],position_left);
	set_tacho_position(sn_wheels[0],position_right);
    multi_set_tacho_speed_sp(sn_wheels, speed);
    multi_set_tacho_position_sp(sn_wheels, angle);
	leftFinalPosition = position_left + angle;
	rightFinalPosition = position_right + angle;
    multi_set_tacho_command_inx(sn_wheels, TACHO_HOLD);
}

/*
@desc :
	* the function enables to ask the motors to run to a certain angle with a certain speed.
	* leftFinalPosition and rightFinalPosition are useful to know the remaining distance the robot has to cover.
	* the function is BLOCKING : the function is risky because obstacles won't be discovered if there are.
@param :
	* int speed : speed of the motor
	* int angle : angle the motor has to do
@author : Benjamin Castellan and Samuel Pierre
@return : void
*/
void goStraight(int speed, int distance){
	initPosition();

    int angle = distance_to_angle(distance);
    synchronisedGoStraight(sn_wheels, speed, angle);
	Sleep(50);
	while(robot_is_moving()){ // waiting until the speed of the two motors has reached 0.
		Sleep(10);
		refreshGlobalPosition();
	}

	refreshPosition();

}

/*
@desc :
	* the function is called while the robot is already moving
	* adapt the speed to the distance the robot still has to cover or to the obstacles the robot is going to meet.
@param :
	* int max_speed : max_speed of the motor during the movement
	* int maxDistance : the distance the robot has to do should not be more than maxDistance (example : an obstacle has to be discovered at maxDistance in front of the robot)
	* int securityDistance : the robot is supposed to stop closed to securityDistance of an obstacle (here only used to ajust the speed)
	* int brakingDistance : the robot will begin to brake at brakingDistance
	* int speedDivider : the max_speed will be divided by this factor along the braking period.
@author : Samuel Pierre
@return : the return value will be 1 if the robot had to shorten his previous path or 0 else.
*/
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
	//printf("Distance remaining : %d \n",distance);
	//printf("Maximum distance that the robot should do : %d \n",maxDistance);
	if(distance>maxDistance && brakingDistance>maxDistance){ // testing if the robot has detected stg forcing the robot to stop earlier than in the forecast.
		distance = maxDistance;
		deltaAngle = distance_to_angle(distance);
		leftFinalPosition = get_left_motor_position() + deltaAngle;
		rightFinalPosition = get_right_motor_position() + deltaAngle;
		distanceMaxDone = 0;
	}
	// Calculate the new speed and send the new order to the motor : regular braking
	int newSpeed = max_speed - (((speedDivider-1)*max_speed/speedDivider)*(brakingDistance-distance))/(brakingDistance-securityDistance);
	multi_set_tacho_speed_sp(sn_wheels, newSpeed);
	multi_set_tacho_position_sp(sn_wheels, deltaAngle);
    multi_set_tacho_command_inx(sn_wheels, TACHO_HOLD);
	return distanceMaxDone;
}

/*
@desc :
	* the function just asks the robot to move forward of a certain distance at a certain speed.
	* this function is a NON-BLOCKING function
@param :
	* int speed : speed of the motor during the movement
	* int distance : distance the robot has to do
@author : Samuel Pierre
@return : void
*/
void goStraight_NonBlocking(int speed, int distance){

    int angle = distance_to_angle(distance);
	synchronisedGoStraight(sn_wheels, speed, angle);

}

/*
@desc :
	* the function just asks the robot to do a rotation of a certain angle at a certain speed.
	* this function is a BLOCKING function -> this will block the current thread until the robot has reached the desired position.
	* this function does not use the gyroscope -> see preciseRotation
	* this function cannot be used directly because the position is not monitored here
@param :
	* int speed : speed of the motor during the movement
	* int angle : relative absolute angle the robot has to do.
@author : Samuel Pierre
@return : void
*/
void rotation(int speed, int angle){

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
	Sleep(50);
	while(robot_is_moving()){ // waiting until the speed of the two motors has reached 0.
		Sleep(50);
	}

}

/*
@desc :
	* the function just asks the robot to do a rotation of a certain angle at a certain speed.
	* this function is a BLOCKING function -> this will block the current thread until the robot has reached the desired position.
	* this function does not use the gyroscope -> see preciseRotation
	* contrary to rotation, this function can be used directly : position monitored
@param :
	* int speed : speed of the motor during the movement
	* int angle : relative absolute angle the robot has to do.
@author : Samuel Pierre
@return : void
*/
void smallRotation(int speed, int angle){

	initPosition();
    rotation(speed, angle);
	TETA1 = (TETA1 + angle)%360;
	TETA = TETA1;
	initPosition();

}

/*
@desc :
	* the function just asks the robot to do a rotation of a certain angle at a certain speed.
	* this function is a BLOCKING function -> this will block the current thread until the robot has reached the desired position.
	* this function uses the gyroscope
	* contrary to rotation, this function can be used directly : position monitored
@param :
	* int speed : speed of the motor during the movement
	* int angle : relative absolute angle the robot has to do.
@author : Camille Morin
@return : void
*/
void preciseRotation(int speed, int angle){

	int angle_gyro_start;
	int angle_gyro_end;
	int difference;
	initPosition();
	angle_gyro_start = getGyroAngle();
	//printf("angle gyro avant : %d \n",(int) getGyroAngle());
	rotation(speed, angle);
	//printf("angle gyro apres : %d \n",(int) getGyroAngle());
	angle_gyro_end = getGyroAngle();

  	difference = angle - (angle_gyro_end - angle_gyro_start);
	//printf("angle restant : %d \n",difference);
	if(difference!=0){
		rotation(speed, difference);
	}

	TETA1 = (TETA1 + angle)%360;
	TETA = TETA1;
	initPosition();

}

/*
@desc :
	* the function just asks the robot to do a rotation of a certain angle at a certain speed.
	* this function is a BLOCKING function -> this will block the current thread until the robot has reached the desired position.
	* this function uses the gyroscope
	* the function does not refresh global positionning variable
@param :
	* int speed : speed of the motor during the movement
	* int angle : relative absolute angle the robot has to do.
@author : Camille Morin
@return : void
*/
void preciseRotation_without_refresh(int speed, int angle){

	int angle_gyro_start;
	int angle_gyro_end;
	int difference;
	angle_gyro_start = getGyroAngle();
	//printf("angle gyro avant : %d \n",(int) getGyroAngle());
	rotation(speed, angle);
	//printf("angle gyro apres : %d \n",(int) getGyroAngle());
	angle_gyro_end = getGyroAngle();

  	difference = angle - (angle_gyro_end - angle_gyro_start);
	//printf("angle restant : %d \n",difference);
	if(difference!=0){
		rotation(speed, difference);
	}

}

/*
@desc :
	* the function enables to establish a save of certain variables before executing a movement.
@param : /
@author : Samuel Pierre
@return : void
*/
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
	turnBack = 0;
}

/*
@desc :
	* the function gives results over a whole movement
@param : /
@author : Samuel Pierre
@return : void
*/
void refreshPosition(){

	int difference;
	int meanAngle;

	if(rotationPolarity==0){ // checking that the movement ongoing is not a rotation

		//printf("Left motor pos=%d\n", (int) get_left_motor_position());
		//printf("Right motor pos=%d\n", (int) get_left_motor_position());
		meanAngle = (fabs(get_left_motor_position()-leftStartPosition)+fabs(get_right_motor_position()-rightStartPosition))/2;
		difference = angle_to_distance(meanAngle);
		if(turnBack==0){
			X1 = X1 + difference*sin(TETA*PI/180);
			Y1 = Y1 + difference*cos(TETA*PI/180);
		}
		else{
			X1 = X1 - difference*sin(TETA*PI/180);
			Y1 = Y1 - difference*cos(TETA*PI/180);
		}
		X=X1;
		Y=Y1;
		initPosition();
	}

}

/*
@desc :
	* Contrary to refreshPosition, this function is designed to have an accurate estimation of the position at every moments
@param : /
@author : Samuel Pierre
@return : void
*/
void refreshGlobalPosition(){
	/*
	refresh the position X,Y and the angle TETA of the robot.
	*/

	int difference;
	int meanAngle;

	if(rotationPolarity==0){

		meanAngle = (int) (fabs(get_left_motor_position()-leftStartPosition)+fabs(get_right_motor_position()-rightStartPosition))/2;
		difference = angle_to_distance(meanAngle);
		if(turnBack==0){
			X = X1 + difference*sin(TETA*PI/180);
			Y = Y1 + difference*cos(TETA*PI/180);
		}
		else{
			X = X1 - difference*sin(TETA*PI/180);
			Y = Y1 - difference*cos(TETA*PI/180);
		}
	}

}

/*
@desc :
	* This function enables to stop the motors
@param : /
@author : Samuel Pierre
@return : void
*/
void stopMotors(){
    multi_set_tacho_position_sp(sn_wheels, 0);
    multi_set_tacho_command_inx(sn_wheels, TACHO_STOP);
	while(robot_is_moving()){ // waiting until the speed of the two motors has reached 0.
		Sleep(10);
	}
	refreshPosition();
}

/*
@desc :
	* This function enables to convert the angle done by a wheel in distance in mm
@param :
	* angle in degree
@author : Benjamin Castellan and Samuel Pierre
@return : (int) distance in mm
*/
int angle_to_distance(int angle){
    return (angle*PI*DIAMETRE)/360;
}

/*
@desc :
	* This function enables to convert the distance done by a wheel in angle in degree
@param :
	* distance in mm
@author : Samuel Pierre
@return : (int) angle in degree
*/
int distance_to_angle(int distance){
    return ((distance*360)/(PI*DIAMETRE));
}

/*
@desc :
	* This function enables to get the max speed of the tacho
@param : /
@author : Samuel Pierre
@return : (int) max_speed of the tacho
*/
int getTachoMaxSpeed(){
    int max_speed;
    get_tacho_max_speed( sn_wheels[0], &max_speed );
    return max_speed;
}
