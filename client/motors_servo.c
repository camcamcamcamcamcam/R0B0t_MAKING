#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

#include "motors_servo.h"
#define PI 3.14159265
#define DIAMETRE 56  // diameter of the wheel : 56mm
#define DIAMETRE_ROBOT 155 // width of the robot in mm
#define ANGLE_ZERO 0 //40 //the horizontal one
#define ANGLE_UP 90 //55 //45
#define MAX_SPEED 1050
#define R 0.857142
uint8_t sn_servo[2];
uint32_t n, ii;

int angle_servo_arm(){
	int angle;
	get_tacho_position(sn_servo[0], &angle);
	return angle;
}

int angle_servo_sonar(){
	int angle;
	get_tacho_position(sn_servo[1], &angle);
	return angle;
}

void servo_arm_up(){
	//int angle;
    //get_tacho_position_sp(sn_servo, &angle);
    //printf("l'angle avant est %d\n", angle);
    goForAngleForever(sn_servo[0],MAX_SPEED / 6, -ANGLE_UP); // else the robot 
	while(angle_servo_arm()<ANGLE_UP){
		Sleep(10);
	}
	//get_tacho_position_sp(sn_servo, &angle);
    //printf("l'angle après est %d\n", angle);
}

void servo_arm_down(){
    goForAngleForever(sn_servo[0],MAX_SPEED / 6, ANGLE_ZERO);
	while(angle_servo_arm()>ANGLE_ZERO){
		Sleep(10);
	}
    //get_tacho_position_sp(sn_servon, &angle);
    //printf("l'angle est %d\n", angle);
}

void servo_sonar(int angle){
    goForAngleForever(sn_servo[1],MAX_SPEED / 6, -angle);
	printf("Angle servo sonar : %d\n",angle_servo_sonar());
	Sleep(100);
	//while(fabs(angle_servo_sonar()-angle)>10){
		//printf("Angle servo sonar : %d\n",angle_servo_sonar());
	//	Sleep(10);
	//}
	//while(angle_servo_sonar()!=angle){
	//	Sleep(10);
	//}
    //get_tacho_position_sp(sn_servon, &angle);
    //printf("l'angle est %d\n", angle);
}

void absolute_servo_sonar(int angle){
	
	int motor_angle;
	motor_angle = angle *(1/R);
	servo_sonar(motor_angle);
	
}

void initMotorServo(){
/*need to be started at the beginning
 Allows to use the motors for the wheels*/

    if ( ev3_init() == -1 ) return ( 1 );
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );//do not remove this line, or the LEGO_EV3_M_MOTOR 1 will NOT be found
    ev3_search_tacho_plugged_in(67, 0, &sn_servo[0], 0);
    ev3_search_tacho_plugged_in(65, 0, &sn_servo[1], 0);
	int initialValue;
	
	printf("Angle actuel du servo bras : \n");
	scanf("%d",&initialValue);
	set_tacho_position(sn_servo[0], initialValue);
	printf("\n");
	servo_arm_down();
	while(initialValue!=0){
		printf("Angle actuel du servo bras : \n");
		scanf("%d",&initialValue);
		set_tacho_position(sn_servo[0], initialValue);
		printf("\n");
		servo_arm_down();
	}
	
	
	printf("Angle actuel du servo sonar : \n");
	scanf("%d",&initialValue);
	set_tacho_position(sn_servo[1], initialValue);
	printf("\n");
	servo_sonar(0);
	while(initialValue!=0){
		printf("Angle actuel du servo sonar : \n");
		scanf("%d",&initialValue);
		set_tacho_position(sn_servo[1], initialValue);
		printf("\n");
		servo_sonar(0);
	}

}

void goForAngleForever(uint8_t sn_servo,int speed, int angle) {
/*only make one wheel turn with the motor on the specified port in the good direction*/
    if (angle > 0){
        set_tacho_polarity_inx(sn_servo,TACHO_INVERSED);
    } else{
        set_tacho_polarity_inx(sn_servo,TACHO_NORMAL);
        angle = -angle;
    }
    set_tacho_speed_sp(sn_servo, speed);
//  set_tacho_ramp_up_sp( sn_servo, 0 );
//  set_tacho_ramp_down_sp( sn_servo, 0 );
	
	set_tacho_position_sp(sn_servo, angle);
	set_tacho_stop_action_inx(sn_servo, TACHO_HOLD);
	set_tacho_command_inx(sn_servo, TACHO_RUN_TO_ABS_POS);
	//set_tacho_command_inx(sn_servo, TACHO_RUN_FOREVER);
	//set_tacho_command_inx(sn_servo, TACHO_RUN_TO_REL_POS);
}

/*
int main( void ){
//to test each function, we need the main

    initMotorServo();
    int max_speed;
    get_tacho_max_speed( sn_servo[0], &max_speed );
    printf("max_speed is %d\n", max_speed);
    //test

    //int tacho_rot;
    //get_tacho_count_per_rot(sn_servo, &tacho_rot);

    //int tacho_m;
    //get_tacho_count_per_m(sn_servo, &tacho_m);
    //printf("tacho rot is : %d and tacho m is : %d \n",tacho_rot,tacho_m);

    //get_motor_position(68);
    //goStraight(sn_servo, max_speed / 12, 210);
    //rotation(sn_servo, max_speed / 12, 720);
	// angle absolu 25 : correspond au 0 attendu
	int i = 0;
    while (i<5){
		servo_arm_up();
		printf("l'angle servo est %d\n", angle_servo_arm());
		Sleep(1000);
        servo_arm_down();
		printf("l'angle servo est %d\n", angle_servo_arm());
		Sleep(1000);
		i=i+1;
    }
	
	i=0;
	while (i<5){
		servo_sonar(45);
		printf("l'angle sonar est %d\n", angle_servo_sonar());
		Sleep(1000);
        servo_sonar(0);
		printf("l'angle sonar est %d\n", angle_servo_sonar());
		Sleep(1000);
		i=i+1;
    }


    //while(1){
    //   Sleep(50);
    //  get_motor_position(68);
    //}

}
*/