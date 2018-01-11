#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include "servo_sonar.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"
#include "robotclient.h"
#include "motors_wheels.h"
#define MAP_HEIGHT 80
#define MAP_WIDTH 80

extern int X; //in mm
extern int Y; //in mm

#define THRESHOLD 50000
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
extern unsigned char map[MAP_WIDTH][MAP_HEIGHT];
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


void client_position(){
  /*
  send the postition to the server every 2 seconds
  update the local map every 50 ms to say there is no obstacle
  */
  char count = 0;
    while(1){

        int x = (int) (X/50);
        int y = (int) (Y/50);
        //x and y must be between 0 and 80 (ie inside the map)
        x = (x < (MAP_WIDTH-1)) ? ((x >= 0) ? x : 0) : (MAP_WIDTH-1);
        y = (y < (MAP_HEIGHT-1)) ? ((y >= 0) ? y : 0 ) : (MAP_HEIGHT-1);
        if (count == 40) {
          sendMessage(MSG_POSITION, x, y, 0, 0, 0, 0);
          count = 0;
        }
        count++;
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
  Discover teh cells situated just in front of the robot and returns a boolean : True if the robot can move forward..
  */

  // Check if the robot won't go outside the arena
  if (((directionY==1) && (globy == (h-2))) || ((directionY==-1) && (globy==2)) || ((directionX==1) && (globx==(w-2))) || ((directionX==-1) && (globx==2))) {
    is_possible = 0;
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

  // assess the cost represented by the use of the function
  increase_cost(2);
  return(is_possible);
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
  if ((map[globx+3*dirX][globy+3*dirY]==200) || (map[globx+3*dirX+null(dirX)][globy+3*dirY+null(dirY)]==200) || (map[globx+3*dirX+2*null(dirX)][globy+3*dirY+null(dirY)]==200) || (map[globx+3*dirX-null(dirX)][globy+3*dirY-null(dirY)]==200) || (map[globx+3*dirX-2*null(dirX)][globy+3*dirY-null(dirY)]==200)) {
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
  if (can_move_forward()){
    globx = X / 50;
    globy = Y / 50;
    go_to_distance_sweep_regular_braking_new(MAX_SPEED / 6, 10000, 100, 60);
    increase_cost(1);
    return 1;
  }
  return 0;
}

int move_forward_until(int max_pos){
  /*"""
  The function enables to move the robot forward until max_pos has been reached or the robot cannot move anymore.
  It returns the number of cells the robot has been able to do.
  """*/
  int i = 0;
  while ((i<max_pos) && can_move_forward()){
    globx = X / 50;
    globy = Y / 50;
    go_to_distance_sweep_regular_braking_new(MAX_SPEED / 6, 50, 60, 40);
    increase_cost(1);
    i++;
  }
  return i;
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

  // we check that the cells assessed are inisde the arena
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
  srand(time(NULL));   // should only be called once
  while (cost<=THRESHOLD){
    if (!disclosed(TETA + 0)){
      move_forward();
    } else {
      if (!disclosed(TETA + 90)){
        //TODO are this the good names ?
        rotate(90);
        move_forward(); //with 5 cm ??
      } else if (!disclosed(TETA - 90)) {
        rotate(-90);
        move_forward();
      } else {
        int result[2];
        nearest_undisclosed_point(result);
        int indexAngle = result[0];
        int minimum = result[1];
        if (minimum == 1000){
          if (can_move_forward()){
            move_forward();
          } else {
            if (rand()*2 < RAND_MAX) rotate(90);
            else rotate(-90);
          }
        } else {
          rotate(indexAngle);
          move_forward_until(minimum);
        }
      }
    }
  }
}



char main (void) {
  /*
  * The robot is doing the algorithm".
  */
  //starting position in arena 1
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

  algo_recursive_b();
}
