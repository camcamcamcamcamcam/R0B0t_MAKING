#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
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

void go_to_distance_no_sweep(int speed, int distance, int securityDistance);

void go_to_distance_sweep(int speed, int distance, int securityDistance,int amplitudeSweep);

void go_to_distance_sweep_regular_braking(int speed, int distance, int securityDistance,int amplitudeSweep);

void go_to_distance_sweep_regular_braking_new(int speed, int distance, int securityDistance,int amplitudeSweep);

void go_to_distance_sweep_regular_braking_new_v2(int speed, int distance, int securityDistance,int amplitudeSweep);