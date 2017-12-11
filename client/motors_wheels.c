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
#define PI 3.14159265
#define DIAMETRE 56  // diameter of the wheel : 56mm
#define DIAMETRE_ROBOT 170 // width of the robot in mm
#define MAX_SPEED 1050

uint8_t sn_wheels[2];
uint8_t sn_gyro;
uint32_t n, ii;


void initMotorWheels(){
/*need to be started at the beginning
 Allows to use the motors for the wheels*/

    if ( ev3_init() == -1 ) return ( 1 );
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );//do not remove this line, or the LEGO_EV3_M_MOTOR 1 will NOT be found
    ev3_search_tacho_plugged_in(66, 0, &sn_wheels[0], 0);
    ev3_search_tacho_plugged_in(68, 0, &sn_wheels[1], 0);

}


void goStraight(int speed, int distance){
/*make both wheels turn with the specified speed and distance in the good direction*/
    int angle = distance_to_angle(distance);
    //goStraightForAngle(sn_wheels[0], speed, angle);
    //goStraightForAngle(sn_wheels[1], speed, angle);
    synchronisedGoStraight(sn_wheels, speed, angle);
}

void rotation(int speed, int angle){
    /*make the robot do a rotation.
     * if angle is positive, it turns to the right
     * if angle is negatve, it turns to the left*/
    int distance_roue = (angle * PI * DIAMETRE_ROBOT) / (360);
    printf("distance roue : %d\n",distance_roue);
    int angle_roue = distance_to_angle(distance_roue);
	printf("angle roue : %d\n",angle_roue);
    goStraightForAngle(sn_wheels[0], speed, angle_roue);
    goStraightForAngle(sn_wheels[1], speed, -angle_roue);
}

void synchronisedGoStraight(uint8_t *sn_wheels, int speed, int angle) {
    /*make the two wheels turn with the motors in the good direction*/
    if (angle > 0){
        multi_set_tacho_polarity_inx(sn_wheels,TACHO_INVERSED);
    } else{
        multi_set_tacho_polarity_inx(sn_wheels,TACHO_NORMAL);
        angle = -angle;
    }
    multi_set_tacho_speed_sp(sn_wheels, speed);
//  set_tacho_ramp_up_sp( sn_wheels, 0 );
//  set_tacho_ramp_down_sp( sn_wheels, 0 );
    multi_set_tacho_position_sp(sn_wheels, angle);
    multi_set_tacho_stop_action_inx(sn_wheels, TACHO_STOP_ACTION__NONE_);
    multi_set_tacho_command_inx(sn_wheels, TACHO_HOLD);
}


void goStraightForAngle(uint8_t sn_wheels, int speed, int angle) {
/*only make one wheel turn with the motor on the specified port in the good direction*/
    if (angle > 0){
        set_tacho_polarity_inx(sn_wheels,TACHO_INVERSED);
    } else{
        set_tacho_polarity_inx(sn_wheels,TACHO_NORMAL);
        angle = -angle;
    }
    printf("angle desire : %d\n",angle);
    set_tacho_speed_sp(sn_wheels, speed);
//  set_tacho_ramp_up_sp( sn_wheels, 0 );
//  set_tacho_ramp_down_sp( sn_wheels, 0 );
    set_tacho_position_sp(sn_wheels, angle);
    set_tacho_stop_action_inx(sn_wheels, TACHO_STOP_ACTION__NONE_);
    set_tacho_command_inx(sn_wheels, TACHO_HOLD);
}

int get_motor_position(int port){
    //1 increment is equal to 1 degree
    uint8_t sn_wheels;
    int position;
    if( ev3_search_tacho_plugged_in(port,0,&sn_wheels,0) ){
        get_tacho_position(sn_wheels,&position);
    }
    printf("port : %d position : %d \n",port,position);
    return position;
}


int angle_to_distance(int angle){
/* take in parameter the difference in angle for the wheels and returns the distance in mm*/
    return ((angle/360)*PI*DIAMETRE);
}


int distance_to_angle(int distance){
/* take in parameter the distance in mm and returns the difference in angle for the wheels */
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

void preciseRotation(int angle,int speed){
    int angle_gyro;
    int difference;
    int compteur;

    rotation(speed,angle);
    initGyro();
    angle_gyro = getAngleGyro();
    difference = angle - angle_gyro;
    printf("angle_gyro = %d, difference = %d\n",angle_gyro,difference);
    compteur = 0;
    while(difference!=0 && compteur <100){
        if (difference <= 180 && difference >= -180) rotation(speed, difference);
        Sleep(1000);
        angle_gyro = getAngleGyro();
        difference = angle - angle_gyro;
        printf("difference = %d, angle_gyro = %d, compteur = %d \n",difference,angle_gyro,compteur);
        compteur++;
    }
}

   
int main( void ){
//to test each function, we need the main

    initMotorWheels(sn_wheels);
    int max_speed;
    int angle;

    get_tacho_max_speed( sn_wheels[0], &max_speed );
    //test

/*    int tacho_rot;
    get_tacho_count_per_rot(sn_wheels, &tacho_rot);

    int tacho_m;
    get_tacho_count_per_m(sn_wheels, &tacho_m);
    printf("tacho rot is : %d and tacho m is : %d \n",tacho_rot,tacho_m);
*/

    //get_motor_position(68);
    //goStraight(max_speed / 2, 594*2);
    //Sleep(2000);
    angle = 180;
    preciseRotation(angle,max_speed);
    while(1){
        Sleep(1000);
        //get_motor_position(68);
        //get_motor_position(66);
    }
    ev3_uninit();

}
