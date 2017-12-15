#include <pthread.h>
#include <signal.h>
#include "servo_sonar.h"
#include "motors_wheels.h"
#include "robotclient.h"

int forward = 0;
int inRotation = 0;

void stop_motor(){
    forward = 0;
    stopMotors();
}

void motor(){

    while(1) {
        if (forward) {
            goStraight(getTachoMaxSpeed() / 5, 10);
        }
        else if(inRotation){
			preciseRotation(getTachoMaxSpeed() / 5, 30);
		}
    }
    return;
}

void sonar(pthread_t * tid_motor){

    while(1){
        if (getDistance(0) > 100) forward = 1;
        int i = -30;
        int sens = 1;
        while(getDistance(i) > 100){
            if (sens) i = i + 5;
            else i = i - 5;
            if (i == 30) sens = 0;
            if (i == -30) sens = 1;
        }
		stop_motor();
        forward = 0;
		inRotation = 1;
        while(getDistance(0) < 100){
			Sleep(10);
        }
		inRotation = 0;
		forward = 1;
    }


}

void client_position(){
    while(1){
        Sleep(2000);
        sendMessage(MSG_POSITION, X, Y, 0, 0, 0, 0);

    }
}


char main (void) {
/*
 * The robot go straight and stop when he detects a wall to close ( < 50 mm )
 */
    initMotorServo();
    initSensorSonar();
    initMotorWheels();
	servo_arm_up();
    //while the server did not send the START_MESSAGE, the robot will wait in init_client()
    // initClient(); // will STOP the program if the server is not lauched !

    pthread_t tid_sonar;
    pthread_attr_t attr_sonar;
    pthread_attr_init(&attr_sonar);

    pthread_t tid_motor;
    pthread_attr_t attr_motor;
    pthread_attr_init(&attr_motor);

	/*
    pthread_t tid_client_position;
    pthread_attr_t attr_client_position;
	pthread_attr_init(&attr_client_position);
	*/
    //signal(SIGALRM, stop_motor);
    signal(SIGALRM, stopMotors);
    signal(SIGCONT, motor);

    char a;
    pthread_create(&tid_motor, &attr_motor, (void *) motor, (void *)&a);
    pthread_create(&tid_sonar, &attr_sonar, (void *) sonar, &tid_motor);
    //pthread_create(&tid_client_position, &attr_client_position, (void *) client_position, (void *)&a);

    int i = 0;
	for (i=0; i < 100; i++){
		Sleep(1000);
		printf("X=%d \n",X);
		printf("Y=%d \n",Y);
		printf("TETA=%d \n",TETA);
    }
    pthread_kill(tid_motor, SIGKILL);       //in case the robot never stop, it kills all the process launched.
    pthread_kill(tid_sonar, SIGKILL);
    //pthread_kill(tid_client_position, SIGKILL);
    return 1;
}
