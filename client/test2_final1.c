//
// Created by benjictln on 17/12/17.
//

#include <pthread.h>
#include <signal.h>
#include "servo_sonar.h"
#include "motors_wheels.h"
#include "motors_servo.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"
#include "robotclient.h"
#define MAP_HEIGHT 80
#define MAP_WIDTH 80

void client_position(){
    while(1){
        /*
        //we send a random map
        int i;
        int j;
        for (i=0; i<20; i++) {
            for (j=0; j<20; j++) {
                sendMessage(MSG_MAPDATA, i, j, 0, 0, 0, 0);
                Sleep(100);
            }
        }
        sendMessage(MSG_MAPDONE,0,0,0,0,0,0);
        */
        Sleep(2000);
        int x = (int) (X/50);
        int y = (int) (Y/50);
        //x and y must be between 0 and 80 (ie inside the map)
        x = (x < (MAP_WIDTH-1)) ? ((x >= 0) ? x : 0) : (MAP_WIDTH-1);
        y = (y < (MAP_HEIGHT-1)) ? ((y >= 0) ? y : 0 ) : (MAP_HEIGHT-1);
        sendMessage(MSG_POSITION, x, y, 0, 0, 0, 0);

    }
}

char main (void) {
/*
 * The robot go straight and stop when he detects a wall to close ( < 50 mm )
 */
    //starting position in arena 1
    X=40;
    Y=10;
    initMotorServo();
    initSensorSonar();
    initMotorWheels();
    servo_arm_up();
    //while the server did not send the START_MESSAGE, the robot will wait in init_client()
    initClient(); // will STOP the program if the server is not launched !

    pthread_t tid_client_position;
    pthread_attr_t attr_client_position;
  	pthread_attr_init(&attr_client_position);
    char a;
    pthread_create(&tid_client_position, &attr_client_position, (void *) client_position, (void *)&a);

    int speed = getTachoMaxSpeed()/5;
    int angle = 90;
    int distance = 10000;
    int securityDistance = 150;
    int amplitudeSweep = 50;

    while(1){
        printf("X=%d\n",X);
        printf("Y=%d\n",Y);
        printf("TETA=%d\n",TETA);
        go_to_distance_sweep(speed, distance, securityDistance, amplitudeSweep);
        printf("X=%d\n",X);
        printf("Y=%d\n",Y);
        printf("TETA=%d\n",TETA);
        printf("AVANT ROTATION");
        rotate_to_angle(speed, angle);
        printf("APRES ROTATION");
    }
    return 1;
}
