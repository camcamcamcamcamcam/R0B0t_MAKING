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

uint8_t sn;
FLAGS_T state;
uint8_t sn_touch;
uint8_t sn_color;
uint8_t sn_compass;
uint8_t sn_sonar;
uint8_t sn_mag;
uint32_t n, ii;



int main( void ){
//to test each function, we need the main


    ev3_search_servo_plugged_in("in2", 0, &sn, 0);

}
