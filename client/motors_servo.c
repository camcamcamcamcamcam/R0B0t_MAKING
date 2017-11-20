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

#include "motors_servo.h"
#define PI 3.14159265
#define DIAMETRE 56  // diameter of the wheel : 56mm
#define DIAMETRE_ROBOT 155 // width of the robot in mm

uint8_t sn;
FLAGS_T state;
uint8_t sn_touch;
uint8_t sn_color;
uint8_t sn_compass;
uint8_t sn_sonar;
uint8_t sn_mag;
uint32_t n, ii;


void initMotorServo(uint8_t *sn){
/*need to be started at the beginning
 Allows to use the motors for the servo*/

    if ( ev3_init() == -1 ) return ( 1 );
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );//do not remove this line, or the LEGO_EV3_M_MOTOR 1 will NOT be found
	ev3_search_tacho_plugged_in(67, 0, sn, 0);

}

void goForAngleForever(uint8_t sn, int speed, int angle) {
/*only make one wheel turn with the motor on the specified port in the good direction*/
    if (angle > 0){
        set_tacho_polarity_inx(sn,TACHO_INVERSED);
    } else{
        set_tacho_polarity_inx(sn,TACHO_NORMAL);
        angle = -angle;
    }
    set_tacho_speed_sp(sn, speed);
//  set_tacho_ramp_up_sp( sn, 0 );
//  set_tacho_ramp_down_sp( sn, 0 );
    set_tacho_position_sp(sn, angle);
	set_tacho_stop_action_inx(sn, TACHO_HOLD);
	set_tacho_command_inx(sn, TACHO_RUN_TO_ABS_POS);
	//set_tacho_command_inx(sn, TACHO_RUN_FOREVER);
}

int main( void ){
//to test each function, we need the main

    initMotorWheels(&sn);
    int max_speed;
    get_tacho_max_speed( sn, &max_speed );
    //test

/*    int tacho_rot;
    get_tacho_count_per_rot(sn, &tacho_rot);

    int tacho_m;
    get_tacho_count_per_m(sn, &tacho_m);
    printf("tacho rot is : %d and tacho m is : %d \n",tacho_rot,tacho_m);
*/

    //get_motor_position(68);
    //goStraight(sn, max_speed / 12, 210);
    //rotation(sn, max_speed / 12, 720);
	// angle absolu 25 : correspond au 0 attendu
	goForAngleForever(sn, max_speed / 5, 17);
    /*while(1){
        Sleep(50);
        get_motor_position(68);
    }*/

}
