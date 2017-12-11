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

uint8_t sn_wheels[2];

void initMotorWheels();
/*need to be started at the beginning
 Allows to use the motors for the wheels*/
void goStraight(int speed, int distance);
/*make both wheels turn with the specified speed and distance in the good direction*/
void goStraightForAngle(uint8_t sn, int speed, int angle);
/*only make one wheel turn with the motor on the specified port in the good direction*/
void stopMotors();
/* stop the motors */
int get_motor_position(int port);

int angle_to_distance(int angle);
/* take in parameter the difference in angle for the wheels and returns the distance in mm*/
int distance_to_angle(int distance);
/* take in parameter the distance in mm and returns the difference in angle for the wheels */
void rotation(int speed, int angle);
    /*make the robot do a rotation.
     * if angle is positive, it turns to the right
     * if angle is negatve, it turns to the left*/
void initGyro();
/*Initialize gyro sensor*/
int getAngleGyro();
/*Returns the angle from the gyro*/
void preciseRotation(int angle, int max_speed);
/*MAke sure the rotation is precise with the gyro*/
int getTachoMaxSpeed();

