﻿/*
     ____    __   ___   ___  _____  ____
    | |__) //\ \ | |_) | |_)  | |  | |_
    |_| \  \\_\/ |_|_) |_|_) _|_|_ |_|__

 *//**
 *  \file  object_interaction.c
 *  \brief  Functions when close to an object.
 */

#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "sensor_sonar.h"
#include "sensor_gyro.h"
#include "ev3_port.h"
#include "ev3_servo.h"
#include "ev3_sensor.h"
#include "color_sensor.h"
#include "motors_servo.h"
#include "motors_wheels.h"
#include "mvt_forward.h"
#include "mvt_rotate.h"
#include "robotclient.h"
#include "map_construction.h"

#ifndef PI
#define PI 3.14159265
#endif
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

#ifndef MAX_SPEED
#define MAX_SPEED 1050
#endif

extern int X; //in mm
extern int Y; //in mm
extern int TETA;

/*
@desc :
	* the function makes the arm go up
	* a message is sent to the server to tell the position of the drop
@param : /
@author : Camille Morin, Louis Roman
@return : void
*/
void dropObject(){
	servo_arm_up();
	int x=(X-150*sin(TETA*PI/180))/50;
	int y=(Y-150*cos(TETA*PI/180))/50;
	setMapData(x,y,100);
	send(MSG_OBSTACLE,x,y,0,0,0,0);
	printf("dropObject sent MSG_OBSTACLE at x=%d y=%d",x,y);
}

/*
@desc :
	* the function takes an object in front of the robot
	* a message is sent to the server to tell the position of the catch
@param : /
@author : Camille Morin, Louis Roman
@return : void
*/
void takeObject(){
	rotate_to_angle(MAX_SPEED/6,180);
	servo_arm_down();
	int y=(Y+200*cos(TETA*PI/180))/50;
	int x=(X+200*sin(TETA*PI/180))/50;
	send(MSG_OBSTACLE,x,y,0,0,0,1); //à améliorer (en prenant en compte le sonar ou en estimant mieux la distance ?)
	printf("takeObjet sent MSG_OBSTACLE at x=%d y=%d",x,y);
	rotate_to_angle(MAX_SPEED/6,-180);
}

/*
@desc :
	* the function tells if it is a movable (red) obstacle
@param : /
@author : Camille Morin, Louis Roman
@return : 1 if movable, 0 if non-movable
*/
int isMovableObstacle(){
/*Fonction pour savoir si l'object devant est un obstacle mobile ou non
 A faire : enregistrer la position de l'objet si ce n'est pas un objet mobile*/
	int a=red_obstacle();
	return(a);
}

/*
@desc :
	* the function tells which way the robot is going (north is initialized as the starting direction of the robot)
@param : /
@author : Camille Morin
@return : int 0 if north, 1 if east, 2 if south, 3 if west
*/
int direction(){
	int angle;
	angle = TETA;
	if(angle<0){
		angle=angle+360;
	}
	int var = (angle+45)/90;
	return var%4;
}

/*
@desc :
	* the function detects in which squares in front of the robot the obstacle is
	* the map is updated accordingly
@param : /
@author : Camille Morin
@return : void
*/
void checkForward(){
	printf("***  BEGINNING CHECKFORWARD  ***\n");
	int X_start = X;
	int Y_start = Y;
	int delta_angle=35;
	int gauche=0;
	int face=0;
	int droite=0;
	int gauche_plus=0;
	int droite_plus=0;
	int delta_x=0;
	int delta_y=0;
	char type = 2;

	//go_to_distance_sweep_regular_braking_new_v2(MAX_SPEED/8, 50, 50, 30);
	//Sleep(100);
	int dir = direction();
	switch(dir)
  {
   case 0: delta_x=1; printf("													dir is nord\n");
	 	break;
   case 1: delta_y=-1;printf("													dir is est\n");
	 	break;
   case 2: delta_x=-1;printf("													dir is sud\n");
	 	break;
   case 3: delta_y=1;printf("													dir is ouest\n");
	  break;
   default: ;
  }
	int x=X/50-3*delta_y;
	int y=Y/50+3*delta_x;
	face = get_sonar_distance();
	if (face<100){
		if(type==2){
			if (isMovableObstacle()){
				type = 100;
			}
			else{
				type = 1;
			}
		}
		printf("setMapData: %d %d %d\n",x,y,type);
			setMapData(x,y,type);

	}
	else{
		  printf("setMapData: %d %d %d\n",x,y,0);
			setMapData(x,y,0);
	}
	absolute_servo_sonar(delta_angle-5);
	printf("angle_servo : %d \n", -delta_angle);
	Sleep(100);
	gauche = get_sonar_distance();
	if (gauche<100){
		if(type==2){
			if (isMovableObstacle()){
				type = 100;
			}
			else{
				type = 1;
			}
		}
		printf("setMapData: %d %d %d\n",x-delta_x,y-delta_y,type);
			setMapData(x-delta_x,y-delta_y,type);

	}
	else{
		  printf("setMapData: %d %d %d\n",x-delta_x,y-delta_y,0);
			setMapData(x-delta_x,y-delta_y,0);
	}
	absolute_servo_sonar(2*delta_angle-10);
	printf("angle_servo : %d \n", -2*delta_angle);
	Sleep(100);
	gauche_plus = get_sonar_distance();
	if (gauche_plus<150){
		if(type==2){
			if (isMovableObstacle()){
				type = 100;
			}
			else{
				type = 1;
			}
		}
		printf("setMapData: %d %d %d\n",x-2*delta_x,y-2*delta_y,type);
		setMapData(x-2*delta_x,y-2*delta_y,type);

	}
	else{
		  printf("setMapData: %d %d %d\n",x-2*delta_x,y-2*delta_y,0);
			setMapData(x-2*delta_x,y-2*delta_y,0);
	}
	absolute_servo_sonar(-delta_angle);

	printf("angle_servo : %d \n", delta_angle);
	Sleep(100);
	droite = get_sonar_distance();
	if (droite<100){
		if(type==2){
			if (isMovableObstacle()){
				type = 100;
			}
			else{
				type = 1;
			}
		}
		printf("setMapData: %d %d %d\n",x+delta_x,y+delta_y,type);
			setMapData(x+delta_x,y+delta_y,type);

	}
	else{
		  printf("setMapData: %d %d %d\n",x+delta_x,y+delta_y,0);
			setMapData(x+delta_x,y+delta_y,0);
	}
	absolute_servo_sonar(-2*delta_angle);
	printf("angle_servo : %d \n", 2*delta_angle);
	Sleep(100);
	droite_plus = get_sonar_distance();
	if (droite_plus<150){
		if(type==2){
			if (isMovableObstacle()){
				type = 100;
			}
			else{
				type = 1;
			}
		}
		printf("setMapData: %d %d %d\n",x+2*delta_x,y+2*delta_y,type);
		setMapData(x+2*delta_x,y+2*delta_y,type);

	}
	else{
		  printf("setMapData: %d %d %d\n",x+2*delta_x,y+2*delta_y,0);
			setMapData(x+2*delta_x,y+2*delta_y,0);
	}
	/*
	if (type==2){
		for (int i=-2; i<=2; i++){
			setMapData(x+i*delta_x,y+i*delta_y,200);
		}
	}
	*/
	absolute_servo_sonar(0);
	//printf("angle_servo : %d \n", 0);
	Sleep(100);
	//printf("angle_gyro : %d \n", getGyroAngle());
	//rotate_to_angle(MAX_SPEED/4,-180);
	//printf("angle_gyro : %d \n", getGyroAngle());
	//int diff = abs(Y-Y_start)+abs(X-X_start);
	//go_to_distance_sweep_regular_braking_new_v2(MAX_SPEED/8,-diff,0,30);
	//rotate_to_angle(MAX_SPEED/4,180);
	printf("***  ENDING CHECKFORWARD  ***\n");

}
