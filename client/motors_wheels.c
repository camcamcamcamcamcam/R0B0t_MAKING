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

uint8_t sn[2];
FLAGS_T state;
uint8_t sn_touch;
uint8_t sn_color;
uint8_t sn_compass;
uint8_t sn_sonar;
uint8_t sn_mag;
uint32_t n, ii;


void initMotorWheels(uint8_t *sn){
/*need to be started at the beginning
 Allows to use the motors for the wheels*/

    if ( ev3_init() == -1 ) return ( 1 );
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );//do not remove this line, or the LEGO_EV3_M_MOTOR 1 will NOT be found
    ev3_search_tacho_plugged_in(66, 0, &sn[0], 0);
    ev3_search_tacho_plugged_in(68, 0, &sn[1], 0);

}


void goStraight(uint8_t *sn, int speed, int distance){  //todo Make then run at the same time
/*make both wheels turn with the specified speed and distance*/
        int angle = distance_to_angle(distance);
        goStraightForAngle(sn[0], speed, angle);
        goStraightForAngle(sn[1], speed, angle);
    }


void goStraightForAngle(uint8_t sn, int speed, int angle) {
/*only make one wheel turn with the motor on the specified port*/

    set_tacho_speed_sp(sn, speed);
//  set_tacho_ramp_up_sp( sn, 0 );
//  set_tacho_ramp_down_sp( sn, 0 );
    set_tacho_position_sp(sn, angle);
    set_tacho_command_inx(sn, TACHO_RUN_TO_REL_POS);
}


int get_motor_position(int port){
    //1 increment is equal to 1 degree
    uint8_t sn;
    int position;
    if( ev3_search_tacho_plugged_in(port,0,&sn,0) ){
        get_tacho_position(sn,&position);
    }
    printf("position : %d \n",position);
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

int main( void ){
//to test each function, we need the main

    initMotorWheels(sn);
    int max_speed;
    ev3_search_tacho_plugged_in(66, 0, sn, 0);
    get_tacho_max_speed( sn[0], &max_speed );
    //test

/*    int tacho_rot;
    get_tacho_count_per_rot(sn, &tacho_rot);

    int tacho_m;
    get_tacho_count_per_m(sn, &tacho_m);
    printf("tacho rot is : %d and tacho m is : %d \n",tacho_rot,tacho_m);
*/

    get_motor_position(68);
    goStraight(sn, max_speed / 12, 210);

    /*while(1){
        Sleep(50);
        get_motor_position(68);
    }*/

}
