#include <pthread.h>
#include <signal.h>
#include "servo_sonar.h"
#include "motors_wheels.h"

int goOn = 0;
char distanceChecked = 0;

void stop_motor(){
    goOn = 0;
    stopMotors();
}


void motor(){
    //struct sched_param param;
    //param.sched_policy = 0;
    //pthread_setschedparam(pthread_self(),SCHED_OTHER, &param);
    while (distanceChecked == 0) Sleep(10);
    while(goOn) {
        goStraight(getTachoMaxSpeed()/12,400);
        Sleep(1000);
    }
    return;
}

void sonar(pthread_t * tid_motor){
    //struct sched_param param;
    //param.sched_policy = 0;
    //pthread_setschedparam(pthread_self(),SCHED_OTHER, &param);
    if (getDistance(0) > 100) goOn = 1;
    distanceChecked = 1;
    int i = -30;
    int sens = 1;
    while(getDistance(i) > 100){
        if (sens) i = i + 5;
        else i = i - 5;
        if (i == 30) sens = 0;
        if (i == -30) sens = 1;
    }
    pthread_kill(*tid_motor,SIGALRM);
}

void main (int argc, char **argv) {
/*
 * The robot go straight and stop when he detects a wall to close ( < 50 mm )
 */
    initMotorServo();
    initSensorSonar();
    initMotorWheels();

    pthread_t tid_sonar;
    pthread_attr_t attr_sonar;
    pthread_t tid_motor;
    pthread_attr_t attr_motor;
    pthread_attr_init(&attr_sonar);
    pthread_attr_init(&attr_motor);
    signal(SIGALRM, stop_motor);

    //struct sched_param param;

    //pthread_attr_getschedparam(&attr_sonar,&param);
    //param.sched_priority = 0;
    //pthread_setschedparam(&attr_sonar,SCHED_OTHER, &param);

    //pthread_attr_getschedparam(&attr_motor,&param);
    //param.sched_priority = 0;
    //pthread_setschedparam(&attr_motor,SCHED_OTHER, &param);

    char a;
    pthread_create(&tid_motor, &attr_motor, (void *) motor, (void *)&a);
    pthread_create(&tid_sonar, &attr_sonar, (void *) sonar, &tid_motor);
    int i = 0;
	for (i=0; i < 10; i++){
		Sleep(1000);
		printf("X=%d \n",X);
		printf("Y=%d \n",Y);
		printf("TETA=%d \n",TETA);
	}
    pthread_kill(tid_motor, SIGKILL);
    pthread_kill(tid_sonar, SIGKILL);
    return;
}
