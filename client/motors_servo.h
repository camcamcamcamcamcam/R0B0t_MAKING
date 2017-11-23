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

void initMotorServo();
/*need to be started at the beginning
 Allows to use the servo motor*/
void goForAngleForever(uint8_t sn, int speed, int angle);
/*only make the servo motor turn until the specified angle has been reached.*/

void servo_up();
/*servo motor turns up*/

void servo_down();
/*servo motor turns horizontal*/

