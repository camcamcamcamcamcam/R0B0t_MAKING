#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "sensor_gyro.h"
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

uint8_t sn_wheels[2];
int X;
int Y;
int TETA;

void initMotorWheels();
/*need to be started at the beginning
 Allows to use the motors for the wheels*/
void goStraight(int speed, int distance);
/*make both wheels turn with the specified speed and distance in the good direction*/
void goStraight_NonBlocking(int speed, int distance);
/*make both wheels turn with the specified speed : non blocking function */
void synchronisedGoStraight(uint8_t *sn_wheels, int speed, int angle);
/* go straight using synchronized method providing by the AP../.. */
void goStraightForAngle(uint8_t sn, int speed, int angle);
/*only make one wheel turn with the motor on the specified port in the good direction*/
void slow_down(int speed);
char manage_speed(int speed, int maxDistance,int securityDistance,int brakingDistance, int speedDivider);
void stopMotors();
/* stop the motors */
int get_left_motor_position();
/* return the position of the left motor */
int get_right_motor_position();
/* return the position of the right motor */
int angle_to_distance(int angle);
/* take in parameter the difference in angle for the wheels and returns the distance in mm*/
int distance_to_angle(int distance);
/* take in parameter the distance in mm and returns the difference in angle for the wheels */
int robot_is_moving();
/* return 1 if the robot is moving and 0 else. */
void rotation(int speed, int angle);
    /*make the robot do a rotation.
     * if angle is positive, it turns to the right
     * if angle is negatve, it turns to the left*/
void refreshPosition();
/* refreshing position at each checkpoint. */
void refreshGlobalPosition();
/* refreshing position at each checkpoint. */
void initPosition();
/*Initializes postion.*/
void preciseRotation(int angle, int speed);
/*MAke sure the rotation is precise with the gyro*/
void smallRotation(int angle, int speed);
int getTachoMaxSpeed();
