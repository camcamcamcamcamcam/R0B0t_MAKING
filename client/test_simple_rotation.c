#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include "servo_sonar.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"
//#include "robotclient.h"
#include "motors_wheels.h"
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
unsigned char map[MAP_WIDTH][MAP_HEIGHT] = { [0 ... MAP_WIDTH-1][0 ... MAP_HEIGHT-1] = 200};
/*
 * the map is made of :
 *   0 if there is nothing
 *   1 if there is a non-moving obstacle
 *   100 if there is a moving obstacle
 *   200 if the value is unknown
 */


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
  //initClient(); // will STOP the program if the server is not launched !

  rotate_to_angle(MAX_SPEED/5,90);
  Sleep(500);
  rotate_to_angle(MAX_SPEED/5,-90);
  return 0;
}
