#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_servo.h"
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
/*
#include "motor_servo.h"
*/
uint8_t * sn;
FLAGS_T state;
uint8_t sn_touch;
uint8_t sn_color;
uint8_t sn_compass;
uint8_t sn_sonar;
uint8_t sn_mag;
uint32_t n, ii;

void initMotorServo(uint8_t * sn){
/*need to be started at the beginning
 Allows to use the SERVO MOTOR*/
    if ( ev3_init() == -1 ) return ( 1 );
    //while ( ev3_servo_init() < 1 ) Sleep( 1000 );//do not remove this line, or the LEGO_EV3_M_MOTOR 1 will NOT be found
    if(ev3_search_servo_plugged_in(67, 0, sn, 0)){
		printf("ok");
	}
	else{
		printf("pas ok");
	}

}

int main( void ){
//to test each function, we need the main
	initMotorServo(sn);
	//goForAngle(*sn,45);
	printf("Hello world");
}
