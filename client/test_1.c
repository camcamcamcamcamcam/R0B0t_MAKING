#include "servo_sonar.h"
#include "motors_wheels.h"


void main (int argc, char **argv){

    initMotorServo();
    initSensorSonar();
    initMotorWheels();

    while(getDistance(0) > 50) {
        goStraight(getTachoMaxSpeed()/12,400);
        Sleep(100);
    }
    stopMotors();

    return;
}