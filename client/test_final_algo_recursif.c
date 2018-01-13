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
int globx;  //coordonnate
int globy;  //coordonnate
extern int TETA;
char directionX;
char directionY;
char is_possible = 1;
char h = MAP_HEIGHT; // the height of the map
char w = MAP_WIDTH; // the width of the map
int cost = 0;
char dirX;
char dirY;
char allDistanceDone = 1;

extern unsigned char map[map_x][map_y];
/*
 * the map is made of :
 *   0 if there is nothing
 *   1 if there is a non-moving obstacle
 *   100 if there is a moving obstacle
 *   200 if the value is unknown
 */

void nearest_undisclosed_point();
int nearest_undisclosed_free(int angle);
void rotate(int angle);
char move_forward();
char disclosed(int angle);
char can_move_forward();
void increase_cost(int value);
char null(char a);
void client_position();
void manage_obstacles();
int longest_undisclosed_position();
void initMap();

void initMap(){
	int i=0;
	int j=0;
	while(i<w){
		while(j<h){
			map[i][j]=200;
			j=j+1;
		}
		i=i+1;
	}
}

void client_position(){
  /*
  send the postition to the server every 2 seconds
  update the local map every 50 ms to say there is no obstacle
  */
  int count = 0;
    while(1){

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
				if (count == 400) {
					sendMapDone();
					count = 0;
				}
        count++;
        int i;
        int j;
        for (i = 0; i < 5; i++){
          for (j = 0; j < 5; j++){
            if ((x-2+i >= 0) && (x-2+i < MAP_WIDTH) && (y-2+j >= 0) && (y-2+j < MAP_HEIGHT)) { //so no seg fault
              map[x-2+i][y-2+j] = 0; // it means there is nothing there
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
  if (((directionY==1) && (globy == (h-2))) || ((directionY==-1) && (globy==2)) || ((directionX==1) && (globx==(w-2))) || ((directionX==-1) && (globx==2))) {
    return 0;
  }
  else{
	  int minBuffer = getMinBufferSonar();
	  if(minBuffer>150){
		  return 1;
	  }
	  else{
		return 0;
	  }
  }

  //The 5 following statements enable to test the 5 cells situated in front of the robot
  /* //TODO:replace with the function of Camille
  if Map[globy+3*directionY-2*null(directionY),globx+3*directionX-2*null(directionX)]==1:
      is_possible = False
      map[globy+3*directionY-2*null(directionY),globx+3*directionX-2*null(directionX)]=128
  else:
      map[globy+3*directionY-2*null(directionY),globx+3*directionX-2*null(directionX)]=255

  if Map[globy+3*directionY-null(directionY),globx+3*directionX-null(directionX)]==1:
      is_possible = False
      map[globy+3*directionY-null(directionY),globx+3*directionX-null(directionX)]=128
  else:
      map[globy+3*directionY-null(directionY),globx+3*directionX-null(directionX)]=255

  if Map[globy+3*directionY,globx+3*directionX]==1:
      is_possible = False
      map[globy+3*directionY,globx+3*directionX]=128
  else:
      map[globy+3*directionY,globx+3*directionX]=255

  if Map[globy+3*directionY+null(directionY),globx+3*directionX+null(directionX)]==1:
      is_possible = False
      map[globy+3*directionY+null(directionY),globx+3*directionX+null(directionX)]=128
  else:
      map[globy+3*directionY+null(directionY),globx+3*directionX+null(directionX)]=255

  if Map[globy+3*directionY+2*null(directionY),globx+3*directionX+2*null(directionX)]==1:
      is_possible = False
      map[globy+3*directionY+2*null(directionY),globx+3*directionX+2*null(directionX)]=128
  else:
      map[globy+3*directionY+2*null(directionY),globx+3*directionX+2*null(directionX)]=255
  */

}

char disclosed(int angle){
  /*
  check if the cells linked to the direction corresponding to the angle have already been disclosed
  */
  angle = angle%360;
  if (angle==0){
    dirX = 0;
    dirY = 1;
  } else if (angle==90){
    dirX = 1;
    dirY = 0;
  } else if (angle==180){
    dirX = 0;
    dirY = -1;
  } else {
    dirX = -1;
    dirY = 0;
  }
  printf("Value disclosed (globx,globy)=(%d,%d) \n",globx,globy);
  printf("Disclosed(%d,%d) ? %d\n",globx+3*dirX-2*null(dirX),globy+3*dirY-2*null(dirY),map[globx+3*dirX-2*null(dirX)][globy+3*dirY-2*null(dirY)]);
  printf("Disclosed(%d,%d) ? %d \n",globx+3*dirX-null(dirX),globy+3*dirY-null(dirY),map[globx+3*dirX-null(dirX)][globy+3*dirY-null(dirY)]);
  printf("Disclosed(%d,%d) ? %d \n",globx+3*dirX,globy+3*dirY,map[globx+3*dirX][globy+3*dirY]);
  printf("Disclosed(%d,%d) ? %d \n",globx+3*dirX+null(dirX),globy+3*dirY+null(dirY),map[globx+3*dirX+null(dirX)][globy+3*dirY+null(dirY)]);
  printf("Disclosed(%d,%d) ? %d \n",globx+3*dirX+2*null(dirX),globy+3*dirY+2*null(dirY),map[globx+3*dirX+2*null(dirX)][globy+3*dirY+2*null(dirY)]);
  if ((map[globx+3*dirX][globy+3*dirY]==200) || (map[globx+3*dirX+2*null(dirX)][globy+3*dirY+2*null(dirY)]==200) || (map[globx+3*dirX+null(dirX)][globy+3*dirY+null(dirY)]==200) || (map[globx+3*dirX-null(dirX)][globy+3*dirY-null(dirY)]==200) || (map[globx+3*dirX-2*null(dirX)][globy+3*dirY-2*null(dirY)]==200)) {
      return 0;
  }else{
      return 1;
  }
}

char move_forward(){
  /*
  The function enables to move the robot 5cm forward if it is possible.
  It returns True if the movement has been possible.
  */
  go_to_distance_sweep_regular_braking_new_v2(MAX_SPEED / 6, 50, 50, 60);
  increase_cost(1);
  return 0;
}

void manage_obstacles(){
  /*
  A compléter avec la fonction de Camille
  */
  map[globx+3*directionX-2*null(directionX)][globy+3*directionY-2*null(directionY)]=1;
  map[globx+3*directionX-null(directionX)][globy+3*directionY-null(directionY)]=1;
  map[globx+3*directionX][globy+3*directionY]=1;
  map[globx+3*directionX+null(directionX)][globy+3*directionY+null(directionY)]=1;
  map[globx+3*directionX+2*null(directionX)][globy+3*directionY+2*null(directionY)]=1;
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX-2*null(directionX),globy+3*directionY-2*null(directionY));
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX-null(directionX),globy+3*directionY-null(directionY));
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX,globy+3*directionY);
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX+null(directionX),globy+3*directionY+null(directionY));
  printf("Noticed (%d,%d) as obstacle\n",globx+3*directionX+2*null(directionX),globy+3*directionY+2*null(directionY));
  return;
}

int move_forward_until(int max_pos){
  /*"""
  The function enables to move the robot forward until max_pos has been reached or the robot cannot move anymore.
  It returns the number of cells the robot has been able to do.
  """*/
  go_to_distance_sweep_regular_braking_new_v2(MAX_SPEED / 6, 50*max_pos, 50, 40);
  printf("Mvt forward Finished\n");
  increase_cost(2);
  return 0;
}

void rotate(int angle){
  /*"""
  The function enables to rotate the robot of a certain angle.
  """*/
  //TETA = (TETA + angle) % 360; Done by the other function now ?
  rotate_to_angle(MAX_SPEED / 6, angle);

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

int longest_undisclosed_position(){
  /*
  The function enables to estimate the longest path the robot can do without bumping into already discovered obstacles or already discovered area
  */
  int angle = TETA;
  if (angle==0){
    dirX = 0;
    dirY = 1;
  } else if (angle==90){
    dirX = 1;
    dirY = 0;
  } else if (angle==180){
    dirX = 0;
    dirY = -1;
  } else {
    dirX = -1;
    dirY = 0;
  }
  int i=0;
  unsigned char case1 = 0;
  unsigned char case2 = 0;
  unsigned char case3 = 0;
  unsigned char case4 = 0;
  unsigned char case5 = 0;

  // we check that the cells assessed are inisde the arena
  if ((globy+(3+i)*dirY+2*null(dirY)<h) && (globy+(3+i)*dirY-2*null(dirY)>0) && (globx+(3+i)*dirX+2*null(dirX)<w) && (globx+(3+i)*dirX-2*null(dirX)>0)){
    case1 = map[globx+(3+i)*dirX-2*null(dirX)][globy+(3+i)*dirY-2*null(dirY)];
    case2 = map[globx+(3+i)*dirX-null(dirX)][globy+(3+i)*dirY-null(dirY)];
    case3 = map[globx+(3+i)*dirX][globy+(3+i)*dirY];
    case4 = map[globx+(3+i)*dirX+null(dirX)][globy+(3+i)*dirY+null(dirY)];
    case5 = map[globx+(3+i)*dirX+2*null(dirX)][globy+(3+i)*dirY+2*null(dirY)];
  }
  // we loop until a wall has been found and an undisclosed place has not been found
  while (((case1==200) || (case2==200) || (case3==200) || (case4==200) || (case5==200)) && (globy+(3+i)*dirY+2*null(dirY)<h) && (globy+(3+i)*dirY-2*null(dirY)>0) && (globx+(3+i)*dirX+2*null(dirX)<w) && (globx+(3+i)*dirX-2*null(dirX)>0)){
    if ((case1!=1) && (case2!=1) && (case3!=1) && (case4!=1) && (case5!=1)){
      i++;
      if ((globy+(3+i)*dirY+2*null(dirY)<h) && (globy+(3+i)*dirY-2*null(dirY)>0) && (globx+(3+i)*dirX+2*null(dirX)<w) && (globx+(3+i)*dirX-2*null(dirX)>0)){
        case1 = map[globx+(3+i)*dirX-2*null(dirX)][globy+(3+i)*dirY-2*null(dirY)];
        case2 = map[globx+(3+i)*dirX-null(dirX)][globy+(3+i)*dirY-null(dirY)];
        case3 = map[globx+(3+i)*dirX][globy+(3+i)*dirY];
        case4 = map[globx+(3+i)*dirX+null(dirX)][globy+(3+i)*dirY+null(dirY)];
        case5 = map[globx+(3+i)*dirX+2*null(dirX)][globy+(3+i)*dirY+2*null(dirY)];
      }
    } else {
        return (i+1);
      }
    }
  return 1000;
}

int nearest_undisclosed_free(int angle){
  /*
  The function returns the biggest number of 5cm-cell with the angle "angle" that the robot can do.
  */
  angle = angle % 360;
  if (angle == 0){
    dirX = 0;
    dirY = 1;
  } else if (angle == 90) {
    dirX = 1;
    dirY = 0;
  } else if (angle == 180) {
    dirX = 0;
    dirY = -1;
  } else {
    dirX = -1;
    dirY = 0;
  }
  int i=0;
  unsigned char case1 = 1;
  unsigned char case2 = 1;
  unsigned char case3 = 1;
  unsigned char case4 = 1;
  unsigned char case5 = 1;

  // we check that the cells assessed are inside the arena
  if ((globy+(3+i)*dirY+2*null(dirY)<h) && (globy+(3+i)*dirY-2*null(dirY)>0) && (globx+(3+i)*dirX+2*null(dirX)<w) && (globx+(3+i)*dirX-2*null(dirX)>0)){
    case1 = map[globx+(3+i)*dirX-2*null(dirX)][globy+(3+i)*dirY-2*null(dirY)];
    case2 = map[globx+(3+i)*dirX-null(dirX)][globy+(3+i)*dirY-null(dirY)];
    case3 = map[globx+(3+i)*dirX][globy+(3+i)*dirY];
    case4 = map[globx+(3+i)*dirX+null(dirX)][globy+(3+i)*dirY+null(dirY)];
    case5 = map[globx+(3+i)*dirX+2*null(dirX)][globy+(3+i)*dirY+2*null(dirY)];
  }
  // we loop until a wall has been found and an undisclosed place has not been found
  while ((case1!=1) && (case2!=1) && (case3!=1) && (case4!=1) && (case5!=1) && (globy+(3+i)*dirY+2*null(dirY)<h) && (globy+(3+i)*dirY-2*null(dirY)>0) && (globx+(3+i)*dirX+2*null(dirX)<w) && (globx+(3+i)*dirX-2*null(dirX)>0)){
    if ((case1!=200) && (case2!=200) && (case3!=200) && (case4!=200) && (case5!=200)){
      i++;
      if ((globy+(3+i)*dirY+2*null(dirY)<h) && (globy+(3+i)*dirY-2*null(dirY)>0) && (globx+(3+i)*dirX+2*null(dirX)<w) && (globx+(3+i)*dirX-2*null(dirX)>0)){
        case1 = map[globx+(3+i)*dirX-2*null(dirX)][globy+(3+i)*dirY-2*null(dirY)];
        case2 = map[globx+(3+i)*dirX-null(dirX)][globy+(3+i)*dirY-null(dirY)];
        case3 = map[globx+(3+i)*dirX][globy+(3+i)*dirY];
        case4 = map[globx+(3+i)*dirX+null(dirX)][globy+(3+i)*dirY+null(dirY)];
        case5 = map[globx+(3+i)*dirX+2*null(dirX)][globy+(3+i)*dirY+2*null(dirY)];
      }
    } else {
        return (i+1);
      }
    }
  return 1000;
}

void nearest_undisclosed_point(int * result){
  //"""return the angle the robot should take to go to the nearest undisclosed point in one of the four direction without obstacle between, 1000 if not exists"""
  int angle = 0;
  int minimum = nearest_undisclosed_free(TETA + angle);
  int indexAngle = angle;
  int dist;
  while (angle < 270){
    angle = angle + 90;
    dist = nearest_undisclosed_free(TETA+angle);
    if (dist < minimum) {
      minimum = dist;
      indexAngle = angle;
    }
  }
  result[0] = indexAngle;
  result[1] = minimum;
}

void algo_recursive_b() {
  //srand(time(NULL));   // should only be called once
  directionX = 0;
  directionY = 1;
  while (cost<=THRESHOLD){
	printf("disclosed in front : %d \n",disclosed(TETA + 0));
    if (!disclosed(TETA + 0)){
	  int dist =  longest_undisclosed_position();
	  printf("Move forward until %d \n",dist);
      move_forward_until(dist);
	  // Test on the five diections : need to be a really accurate test !! -> discover 5 cells in the front
	  //int is_possible = can_move_forward();  uncomment with function of Camille
	  printf("can move forward ? %d \n",is_possible);
	  //if(!is_possible){  uncomment with function of Camille
		if(!allDistanceDone){
		  //procédure gestion obstacles
		  manage_obstacles();  uncomment with function of Camille
	  }
    } else {
		printf("Try to see in the sides \n");
      if (!disclosed(TETA + 90)){
		  printf("i go to +90 \n");
		  rotate(90);
        //move_forward();
      } else if (!disclosed(TETA - 90)) {
		  printf("i go to -90 \n");
        rotate(-90);
        //move_forward();
      } else {
		  printf("i have to analyze \n");
        int result[2];
        nearest_undisclosed_point(result);
        int indexAngle = result[0];
        int minimum = result[1];
        if (minimum == 1000){
          //if (can_move_forward()){
					if (!allDistanceDone){
					  move_forward();
          } else {
            if (rand()*2 < RAND_MAX) rotate(90);
            else rotate(-90);
          }
        } else {
          rotate(indexAngle);
          move_forward_until(minimum);
		  //if(!can_move_forward()){
			if(!allDistanceDone){  //to comment with function of Camille
			  //procédure gestion obstacles
			  manage_obstacles();
		  }
        }
      }  uncomment with function of Camille
    }
  }
}



void main (void) {
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
  initClient(); // will STOP the program if the server is not launched !

  pthread_t tid_client_position;
  pthread_attr_t attr_client_position;
  pthread_attr_init(&attr_client_position);
  char a;
  pthread_create(&tid_client_position, &attr_client_position, (void *) client_position, (void *)&a);

  algo_recursive_b();
	return 1;
}
