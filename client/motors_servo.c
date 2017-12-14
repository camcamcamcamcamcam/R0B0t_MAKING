#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

#include "motors_servo.h"

#ifndef PI
#define PI 3.14159265
#endif

// Define the variable important for the servo using the arm.
#define ANGLE_ZERO 0
#define ANGLE_UP 90

// Max speed of the tacho motor of the ev3 dev
#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

// Reduction ratio between the gears for the servo using the sonar
#define R 0.857142 

// Global variables
uint8_t sn_servo[2];
uint32_t n, ii;
int polarity_servo;

void initMotorServo(){
	/*
	Initializing 2 servo motors :
	- the one used to move the sonar : servo_sonar
	- the one used to catch obstacles : servo_arm
	*/

    if ( ev3_init() == -1 ) return ( 1 );
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );//do not remove this line, or the LEGO_EV3_M_MOTOR 1 will NOT be found
    ev3_search_tacho_plugged_in(67, 0, &sn_servo[0], 0);
    ev3_search_tacho_plugged_in(65, 0, &sn_servo[1], 0);
	
	/*
	Initialization of the absolute position "0" of the servo_arm
	We ask the user the position of the servo_arm until this one moves to 0.
	-90° corresponds to the up position
	-0° corresponds to the down position
	*/
	printf("/// Initializing the servo_arm of the robot. /// \n");
	int initialValue;
	initialValue=1;
	servo_arm_down();
	
	while(initialValue!=0){
		printf("Current angle of the servo_arm : \n");
		scanf("%d",&initialValue);
		set_tacho_position(sn_servo[0], initialValue);
		printf("\n");
		servo_arm_down();
	}
	initialValue=1;
	printf("/// End of initialization. /// \n");
	Sleep(1000);
	//End Initialization of the servo_arm of the robot
	
	/*
	Initialization of the absolute position "0" of the servo_sonar
	We ask the user the position of the servo_sonar until this one moves to 0.
	0° : the sonar_arm is looking forward
	*/
	printf("/// Initializing the servo_sonar of the robot. /// \n");
	absolute_servo_sonar(0);
	
	while(initialValue!=0){
		printf("Current angle of the servo_sonar : \n");
		scanf("%d",&initialValue);
		set_tacho_position(sn_servo[1], initialValue);
		printf("\n");
		absolute_servo_sonar(0);
	}
	//End Initialization of the servo_sonar of the robot
}

int angle_servo_arm(){
	/*
	Detect the asolute angle of the servo using the arm
	Return an integer : the angle detected in degree.
	*/
	int angle;
	get_tacho_position(sn_servo[0], &angle);
	return angle;
}

int angle_servo_sonar(){
	/*
	Detect the asolute angle of the servo using the sonar
	Return an integer : the angle detected in degree.
	*/
	int angle;
	get_tacho_position_sp(sn_servo[1], &angle);
	return polarity_servo*angle*R;
}

void servo_arm_up(){
	/*
	The function enables to put the servo using the arm at the angle ANGLE_UP (usually 90°).
	It is a blocking function : the thread including this function won't continue before the servo has reached the desired position.
	*/
	
    go_to_angle(sn_servo[0],MAX_SPEED / 6, ANGLE_UP); // launchs the motor
	
	while(servo_arm_is_running()){ // waiting until the speed of the motor has reached 0.
		Sleep(10);
	}
	
}

void servo_arm_down(){
	/*
	The function enables to put the servo using the arm at the angle ANGLE_DOWN (usually 0°).
	It is a blocking function : the thread including this function won't continue before the servo has reached the desired position.
	*/
	
    go_to_angle(sn_servo[0],MAX_SPEED / 6, ANGLE_ZERO);
	
	while(servo_arm_is_running()){ // waiting until the speed of the motor has reached 0.
		Sleep(10);
	}
	
}

int servo_arm_is_running(){
	/*
	The function returns 0 if the servo_arm is not running and 1 else.
	*/
	
	int speed;
	get_tacho_speed( sn_servo[0], &speed);
	if(speed==0){
		return 0;
	}
	else{
		return 1;
	}
	
}

int servo_sonar_is_running(){
	/*
	The function returns 0 if the servo_arm is not running and 1 else.
	*/
	
	int speed;
	get_tacho_speed( sn_servo[1], &speed);
	if(speed==0){
		return 0;
	}
	else{
		return 1;
	}
	
}

void servo_sonar(int angle){
	/*
	The function enables to move the servo using the sonar to the specified angle "angle".
	It is a blocking function : the thread including this function won't continue before the servo has reached the desired position.
	*/
	
    go_to_angle(sn_servo[1],MAX_SPEED / 5, angle);
	
	while(servo_sonar_is_running()){ // waiting until the speed of the motor has reached 0.
		Sleep(10);
	}
	
}

void absolute_servo_sonar(int angle){
	/*
	Some gears are placed between the sonar and the servo_sonar.
	We have to take the reduction ratio in account.
	The function enables to move the sonar to the specified angle "angle".
	*/
	
	int motor_angle;
	motor_angle = angle *(1/R);
	servo_sonar(motor_angle);
	
}

void go_to_angle(uint8_t sn_servo,int speed, int angle) {
	/*
	The function moves the specified servo to a certain angle
	*/
	
	// taking the sign of the angle into account
    if (angle >= 0){
        set_tacho_polarity_inx(sn_servo,TACHO_NORMAL);
		polarity_servo = 1;
    } else{
        set_tacho_polarity_inx(sn_servo,TACHO_INVERSED);
        angle = -angle;
		polarity_servo = -1;
    }
	
    set_tacho_speed_sp(sn_servo, speed); // defining the speed of the motor
	set_tacho_position_sp(sn_servo, angle); // defining the desired position.
	set_tacho_stop_action_inx(sn_servo, TACHO_HOLD);
	set_tacho_command_inx(sn_servo, TACHO_RUN_TO_ABS_POS); // running the motor to the absolute position
}