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

void initMotorWheels();
/*need to be started at the beginning
 Allows to use the motors for the wheels*/
void goStraightForAngleBoth(int speed, int angle);
/*make both wheels turn with the specified speed and angle*/
void goStraightForAngle(int port, int speed, int angle);
/*only make one wheel turn with the motor on the specified port*/

