#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "servo_sonar.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"
#include "robotclient.h"
#include "motors_wheels.h"
#include "map_construction.h"
#include "sensor_sonar.h"
#include "motors_servo.h"
#include "object_interaction.h"
#define MAP_HEIGHT 80
#define MAP_WIDTH 80

// Max speed of the tacho motor of the ev3 dev
#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

extern int X; //in mm
extern int Y; //in mm

#define THRESHOLD 5000
#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif
int globx;  //coordinate
int globy;  //coordinate
extern int TETA;
int directionX;
int directionY;
char is_possible = 1;
char h = MAP_HEIGHT; // the height of the map
char w = MAP_WIDTH; // the width of the map
int cost = 0;
int dirX;
int dirY;

/*
 * the map is made of :
 *   0 if there is nothing
 *   1 if there is a non-moving obstacle
 *   100 if there is a moving obstacle
 *   200 if the value is unknown
 */

void rotate(int angle);
char move_forward();
char can_move_forward();
void increase_cost(int value);
char null(char a);
void client_position();
void manage_obstacles();
void initSmallStadium();

char checkBoundaries(int x_check, int y_check){
	if (x_check >= MAP_WIDTH || x_check < 0 || y_check >= MAP_HEIGHT || y_check < 0)
		return 0;
	return 1;
}

void initSmallStadium(){
	int i;
	int j;
	for (i=0; i <= 40 ; i++){
		setMapData(0, i, 1);
		setMapData(24, i, 1);
	}
	for (j=0; j <= 24; j++){
		setMapData(j,0,1);
		setMapData(j, 40, 1);
	}
}
void client_position(){
  /*
  send the position to the server every 2 seconds
  update the local map every 50 ms to say there is no obstacle
  */
   uint32_t count = 0;
    while(!server_said_stop){

        int x = (int) (X/50);
        int y = (int) (Y/50);
				globx = x;
				globy = y;
				//printf("X=%d\n",X);
				//printf("Y=%d\n",Y);
				//printf("globx=%d\n",globx);
				//printf("globy=%d\n",globy);
        //x and y must be between 0 and 80 (ie inside the map)
        x = (x < (MAP_WIDTH-1)) ? ((x >= 0) ? x : 0) : (MAP_WIDTH-1);
        y = (y < (MAP_HEIGHT-1)) ? ((y >= 0) ? y : 0 ) : (MAP_HEIGHT-1);
        if (count%40 == 0) {
          sendMessage(MSG_POSITION, x, y, 0, 0, 0, 0);
        }
				//if (count == 400) { // 224000
					sendMapDone();
				//}
				if (count == 4700){
					printf("*\n*\n*\n*\n\tALMOST END OF TIME, MAP IS SENDING NOW !!\n*\n*\n*\n");
					sendMapDone();
					count = 0;
				}
        count++;
        int i;
        int j;
        for (i = 0; i < 5; i++){
          for (j = 0; j < 5; j++){
            if ((x-2+i >= 0) && (x-2+i < MAP_WIDTH) && (y-2+j >= 0) && (y-2+j < MAP_HEIGHT)) { //so no seg fault
							setMapData(x-2+i, y-2+j, 0);// it means there is nothing there
							//map[x-2+i][y-2+j] = 0;
            }
          }
        }
        Sleep(50);

    }
}

char null(char a){
    /*
    return 1 if a=0 and 0 else
    */
    if (a==0) return 1;
    return 0;
}

void increase_cost(int value){
  // increase the cost function by the value 'value'. Updating costList and undiscoveredList as well.
  cost += value;
  }

char can_move_forward() {
  /*
  Discover the cells situated just in front of the robot and returns a boolean : True if the robot can move forward..
  */

  // assess the cost represented by the use of the function
  increase_cost(2);
  // Check if the robot won't go outside the arena
  if (((directionY==1) && (globy >= (h-7))) || ((directionY==-1) && (globy<=7)) || ((directionX==1) && (globx>=(w-7))) || ((directionX==-1) && (globx<=7))) {
		printf("*# can_move_forward it was not IN the 80x80 grid\n");
		return 0;
	}
	else{
		int distance_sonar = getMinDistance(75,15);
		printf("*# can_move_forward distance_sonar : %d mm\n", distance_sonar);
		if (distance_sonar < 100)
			return 0;
	}
	return 1;
}

char move_forward(){
  /*
  The function enables to move the robot 5cm forward if it is possible.
  It returns True if the movement has been possible.
  */
  char allDistanceDone = go_to_distance_sweep_regular_braking_new_v2(MAX_SPEED / 4, 50, 50, 60);
  increase_cost(1);
  return allDistanceDone;
}

void manage_obstacles(){
  /*
  A compléter avec la fonction de Camille
  */
	checkForward();
	/*
  map[globx+3*directionX-2*null(directionX)][globy+3*directionY-2*null(directionY)]=1;
  map[globx+3*directionX-null(directionX)][globy+3*directionY-null(directionY)]=1;
  map[globx+3*directionX][globy+3*directionY]=1;
  map[globx+3*directionX+null(directionX)][globy+3*directionY+null(directionY)]=1;
  map[globx+3*directionX+2*null(directionX)][globy+3*directionY+2*null(directionY)]=1;
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX-2*null(directionX),globy+3*directionY-2*null(directionY));
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX-null(directionX),globy+3*directionY-null(directionY));
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX,globy+3*directionY);
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX+null(directionX),globy+3*directionY+null(directionY));
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX+2*null(directionX),globy+3*directionY+2*null(directionY));*/
  return;
}

int move_forward_until(int max_pos){
  /*"""
  The function enables to move the robot forward until max_pos has been reached or the robot cannot move anymore.
  It returns the number of cells the robot has been able to do.
  """*/
  char allDistanceDone = go_to_distance_sweep_regular_braking_new_v2(MAX_SPEED / 4, 50*max_pos, 50, 40);
  printf("Mvt forward Finished, char allDistanceDone is %d\n", allDistanceDone);
  increase_cost(2);
  return allDistanceDone;
}

void rotate(int angle){
  /*"""
  The function enables to rotate the robot of a certain angle.
  """*/
  //TETA = (TETA + angle) % 360; Done by the other function now ?
  rotate_to_angle(MAX_SPEED / 10, angle);

  if (TETA == 0) {
    directionX = 0;
    directionY = 1;
  } else if (TETA == 90) {
    directionX = 1;
    directionY = 0;
  } else if (TETA == 180) {
    directionX = 0;
    directionY = -1;
  } else {
    directionX = -1;
    directionY = 0;
  }
  increase_cost(1);
}

void algo_chenille() {
    srand(time(NULL));   // should only be called once
    directionX = 0;
    directionY = 1;
		int distance;
		char nb = 0;
	  char allDistanceDone = 1;
		char terminated = 0;
    while (cost<=THRESHOLD && !server_said_stop){
				nb = 0;
				terminated = 0;
				while (!terminated && !server_said_stop) {
			  		while(can_move_forward()){
								if(TETA==0){
									distance = 79-(Y/50) -2;
								}
								else if (TETA == 90){
									distance = 79 - (X/50)-2;
								}
								else if (TETA == 180){
									distance = Y / 50-2;
								}
								else {
									distance = X / 50-2;
								}
								move_forward_until(distance);
								if (!can_move_forward()) {
									manage_obstacles();
								}
						}
						rotate(90);
						if (can_move_forward()) {
								allDistanceDone = move_forward_until(5);
								if (!allDistanceDone){
										nb++;
								}
								if (!allDistanceDone && !can_move_forward()) {
										manage_obstacles();
								}
						}
						else {
							manage_obstacles();
							nb++;
						}
						rotate(90);

						while(can_move_forward()){
								if(TETA==0){
									distance = 79-(Y/50)-2;
								}
								else if (TETA == 90){
									distance = 79 - (X/50)-2;
								}
								else if (TETA == 180){
									distance = Y / 50 -2;
								}
								else {
									distance = X / 50 -2;
								}
								move_forward_until(distance);
								if (!can_move_forward()) {
									manage_obstacles();
								}
						}
						rotate(-90);
						if (can_move_forward()) {
								allDistanceDone = move_forward_until(5);
								if (!allDistanceDone){
										nb++;
								}
								if (!allDistanceDone && !can_move_forward()) {
										manage_obstacles();
								}
						}
						else {
							manage_obstacles();
							nb++;
						}
						rotate(-90);
						if (nb == 2){
								terminated = 1;
						}
						nb = 0;
				}
				rotate(-90);
    }

}



int main (void) {
  /*
  * The robot is doing the algorithm".
  */
  //starting position in arena 1
  initMotorServo();
  initSensorSonar();
  initMotorWheels();
  initGyro();
  servo_arm_up();
  //initMap();
  //while the server did not send the START_MESSAGE, the robot will wait in init_client()
	//initSmallStadium();
  initClient(); // will STOP the program if the server is not launched !

  globx = X/50;
  globy = Y/50;

  pthread_t tid_client_position;
  pthread_attr_t attr_client_position;
  pthread_attr_init(&attr_client_position);
  char a;
	pthread_create(&tid_client_position, &attr_client_position, (void *) client_position, (void *)&a);

	pthread_t tid_receive_message;
	pthread_attr_t attr_receive_message;
	pthread_attr_init(&attr_receive_message);
	char b;
  pthread_create(&tid_receive_message, &attr_receive_message, (void *) receiveMessageServer, (void *)&b);

  algo_chenille();
	return 1;
}
