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

uint8_t sn_servo[2];

void initMotorServo();
/*need to be started at the beginning
 Allows to use the servo motor*/
void go_to_angle(uint8_t sn_servo, int speed, int angle);
/*only make the servo motor turn until the specified angle has been reached.*/

int angle_servo_arm();
/*get the value of the current angle of the motor of the arm.*/

int angle_servo_sonar();
/*get the value of the current angle of the motor linked to the sonar.*/

void servo_arm_up();
/*servo motor turns up*/

void servo_arm_down();
/*servo motor turns horizontal*/

void servo_sonar(int angle);
/* make the servo motor linked to the sonar turn from a certain angle. */

void absolute_servo_sonar(int angle);
/* make the sonar turn from a certain angle. */

int servo_sonar_is_running();
/* returns 1 if the servo corresponding to the sonar is running. 0 else. */

int servo_arm_is_running();
/* returns 1 if the servo corresponding to the arm is running. 0 else. */

void stop_servo_sonar();
/* stop the servo motor linked to the sonar. */
