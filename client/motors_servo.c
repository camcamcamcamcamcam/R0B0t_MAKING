#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "motors_servo.h"
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

// Define the variable important for the servo using the arm.
#define ANGLE_ZERO 0
#define ANGLE_UP 90

// Max speed of the tacho motor of the ev3 dev
#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

// Global variables
uint8_t sn_servo[2];
uint32_t n, ii;
int polarity_servo;
pthread_t tid_sweep;
pthread_attr_t attr_sweep;
int sweep_state;
int thread_is_alive;

/*
@desc :
	* initialize the position of the servo motors (the one linked with the sonar and the one linked with the arm).
	* the function has to be called before using the servo motors
@param : /
@author : Samuel Pierre
@return : void
*/
void initMotorServo(){
	/*
	Initializing 2 servo motors :
	- the one used to move the sonar : servo_sonar
	- the one used to catch obstacles : servo_arm
	*/

    if ( ev3_init() == -1 ) return ( 1 );
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );
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
	thread_is_alive = 0;
	//End Initialization of the servo_sonar of the robot
}

/*
@desc : get the current angle of the angle of the servo_arm
@param : /
@author : Samuel Pierre
@return : return the current absolute angle in degrees (int) of the servo_arm
*/
int angle_servo_arm(){
	int angle;
	get_tacho_position(sn_servo[0], &angle);
	return angle;
}

/*
@desc : get the current angle of the angle of the servo_sonar
@param : /
@author : Samuel Pierre
@return : return the current absolute angle in degrees (int) of the servo_sonar
*/
int angle_servo_sonar(){
	int angle;
	get_tacho_position_sp(sn_servo[1], &angle);
	return polarity_servo*angle;
}

/*
@desc :
	* set the servo to the "up" position (perpendicular to the ground), equivalent to 90°.
	* the variable count is there to ensure that the thread currently used won't be blocked if the servo_arm is blocked.
	* the speed is not a parameter and is set to MAX_SPEED / 6.
@param : /
@author : Samuel Pierre
@return : void
*/
void servo_arm_up(){

	int count = 0;
    go_to_angle(sn_servo[0],MAX_SPEED / 6, ANGLE_UP); // launches the motor
	Sleep(50); // make sure that the motor has started before entering the loop
	while(servo_arm_is_running() && count<10){ // waiting until the speed of the motor has reached 0.
		Sleep(20);
		count++;
	}

}

/*
@desc :
	* set the servo to the "down" position (parallel to the ground), equivalent to 0°.
	* the variable count is there to ensure that the thread currently used won't be blocked if the servo_arm is blocked.
	* the speed is not a parameter and is set to MAX_SPEED / 6.
@param : /
@author : Samuel Pierre
@return : void
*/
void servo_arm_down(){

	int count = 0;
    go_to_angle(sn_servo[0],MAX_SPEED / 6, ANGLE_ZERO);
	Sleep(50); // make sure that the motor has started before entering the loop
	while(servo_arm_is_running() && count<100){ // waiting until the speed of the motor has reached 0.
		Sleep(20);
		count++;
	}

}

/*
@desc : the function assesses if the servo_arm is running or not.
@param : /
@author : Samuel Pierre
@return : returns 1 if the servo_arm is running and 0 else.
*/
int servo_arm_is_running(){

	int speed;
	get_tacho_speed( sn_servo[0], &speed);
	if(speed==0){
		return 0;
	}
	else{
		return 1;
	}

}

/*
@desc : the function assesses if the servo_sonar is running or not.
@param : /
@author : Samuel Pierre
@return : returns 1 if the servo_sonar is running and 0 else.
*/
int servo_sonar_is_running(){

	int speed;
	get_tacho_speed( sn_servo[1], &speed);
	if(speed==0){
		return 0;
	}
	else{
		return 1;
	}

}

/*
@desc : the function enables to move the servo_sonar to a certain angle.
@param :
	* int angle : absolute angle desired by the user
	* the variable count is there to ensure that the thread currently used won't be blocked if the servo_arm is blocked.
	* the speed is not a parameter and is set to MAX_SPEED / 10.
@author : Samuel Pierre
@return : void
*/
void absolute_servo_sonar(int angle){

	int count = 0;
    go_to_angle(sn_servo[1],MAX_SPEED / 5, angle);
	Sleep(50); // make sure that the motor has started before entering the loop
	while(servo_sonar_is_running() && count<100){ // waiting until the speed of the motor has reached 0.
		Sleep(20);
		count++;
	}

}

/*
@desc : the function enables to get the current absolute angle of the servo_sonar
@param : /
@author : Samuel Pierre
@return : angle (int) in degrees of the servo_sonar
*/
int get_absolute_angle_servo(){

	int angle;
	get_tacho_position(sn_servo[1],&angle);
	return (int) angle;

}

/*
@desc : the function enables to make the servo motor to go to an absolute angle with a certain speed
@param :
	* uint8_t sn_servo_local : pointer of the servo concerned by this order
	* int speed : speed of the motor
	* int angle : angle the motor has to do
@author : Samuel Pierre
@return : void
*/
void go_to_angle(uint8_t sn_servo_local,int speed, int angle) {

	// taking the sign of the angle into account
    if (angle >= 0){
        set_tacho_polarity_inx(sn_servo_local,TACHO_NORMAL);
		polarity_servo = 1;
    } else{
        set_tacho_polarity_inx(sn_servo_local,TACHO_INVERSED);
        angle = -angle;
		polarity_servo = -1;
    }

    set_tacho_speed_sp(sn_servo_local, speed); // defining the speed of the motor
	set_tacho_position_sp(sn_servo_local, angle); // defining the desired position.
	set_tacho_stop_action_inx(sn_servo_local, TACHO_HOLD);
	set_tacho_command_inx(sn_servo_local, TACHO_RUN_TO_ABS_POS); // running the motor to the absolute position
}

/*
@desc :
	* the thread is there to enable the servo_sonar to sweep
	* it creates the thread if it does not exist
	* it set sweep_state to 1 threw start_sweep() -> to launch this type of movement.
@param : /
@author : Louis Roman
@return : void
*/
void thread_sweep(){

	char a;
	start_sweep();
	if(thread_is_alive == 0){
		pthread_attr_init(&attr_sweep);
		pthread_create(&tid_sweep, &attr_sweep, (void *) continuous_sweep, (void *)&a);
		thread_is_alive = 1;
	}

}

/*
@desc :
	* the function enables to put sweep_state to 1 which will lead to the beginning of the sweep.
@param : /
@author : Louis Roman
@return : void
*/
void start_sweep(){
	sweep_state = 1;
}

/*
@desc :
	* the function enables to make the sonar sweeping or not judging from the value of sweep_state
	* the amplitude of the sweep is set with the local varibale amplitudeAngle
@param : /
@author : Louis Roman
@return : void
*/
void continuous_sweep(){
	int amplitudeAngle = 60;
	while(1){
		//printf("Sweep_state: %d\n",sweep_state);
		if(sweep_state==1){
			//printf("Beginning of the sweep\n");
			absolute_servo_sonar(-amplitudeAngle);
			absolute_servo_sonar(amplitudeAngle);
		}
		else {
			Sleep(100);
			if(sweep_state == 2){
				return;
			}
		}
	}
}

/*
@desc :
	* the function enables to put sweep_state to 0 which will lead to the stop of the sweep.
@param : /
@author : Louis Roman
@return : void
*/
void stop_sweep(){
	sweep_state = 0;
}

/*
@desc :
	* the function enables to exit the thread enabling to make the sonar sweep.
@param : /
@author : Louis Roman
@return : void
*/
void end_thread_sweep(){

	sweep_state = 2;
	pthread_exit(&tid_sweep);
	printf("Thread exit\n");
}
