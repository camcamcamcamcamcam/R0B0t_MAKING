#include <pthread.h>
#include <signal.h>
#include "servo_sonar.h"
#include "motors_wheels.h"
//#include "robotclient.h"

int goOn = 0;
char distanceChecked = 0;
char hasTurned = 0;
int distanceIs;

void stop_motor(){
    goOn = 0;
    stopMotors();
}


void motor(){
    while (distanceChecked == 0) Sleep(10);

    while(1) {
        while (goOn) {
            goStraight(getTachoMaxSpeed() / 8, 400);
            Sleep(1000);
        }
        while (!goOn) Sleep(20);
    }
    return;
}

void let_robot_rotate(){
    /*
     * The robot rotate of 90 degrees and check if he can go on
     */
    stopMotors();
    while ( distanceIs < 100 ){
        hasTurned = 0;
        rotation(getTachoMaxSpeed()/6, 90);
        Sleep(10);
        while(robot_is_moving()) Sleep(10);
        hasTurned = 1;
        Sleep(20);
    }

}

void sonar(pthread_t * tid_motor){

    while(1){
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
        while((distanceIs = getDistance(0)) < 200){
            while (!hasTurned) {
                Sleep(10);
            } //wait the robot has turned
        }
    }


}

/*void client_position(){ //TODO: code the init_function and make sure it works
    init_client();
    while(1){
        Sleep(2000);
        sendMessage(MSG_POSITION, X, Y, 0, 0, 0, 0);

    }
}
*/

void main (int argc, char **argv) {
/*
 * The robot go straight and stop when he detects a wall to close ( < 50 mm )
 */
    initMotorServo();
    initSensorSonar();
    initMotorWheels();

    pthread_t tid_sonar;
    pthread_attr_t attr_sonar;
    pthread_attr_init(&attr_sonar);

    pthread_t tid_motor;
    pthread_attr_t attr_motor;
    pthread_attr_init(&attr_motor);

    //pthread_t tid_client_position;
    //pthread_attr_t attr_client_position;
    //pthread_attr_init(&attr_client_position);
    //signal(SIGALRM, stop_motor);
    signal(SIGALRM, let_robot_rotate);
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
        printf("distance is %d\n",distanceIs);
    }
    pthread_kill(tid_motor, SIGKILL);       //in case the robot never stop, it kills all the process launched.
    pthread_kill(tid_sonar, SIGKILL);
    pthread_kill(tid_client, SIGKILL);
    return;
}
