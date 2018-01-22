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

int angle_servo_arm();

int angle_servo_sonar();

void servo_arm_up();

void servo_arm_down();

int servo_arm_is_running();

int servo_sonar_is_running();

void absolute_servo_sonar(int angle);

int get_absolute_angle_servo();

void go_to_angle(uint8_t sn_servo_local,int speed, int angle);

void thread_sweep();

void start_sweep();

void continuous_sweep();

void stop_sweep();

void end_thread_sweep();