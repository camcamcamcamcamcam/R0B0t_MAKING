/*
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

void dropObject(){
/*Fonction pour relacher un objet
 A faire : enregistrer la position de l'objet lâché sur la carte.*/
	servo_arm_up();
	int x=(X-150*sin(TETA*PI/180))/50;
	int y=(Y-150*cos(TETA*PI/180))/50;
	//send("MSG_OBSTACLE",x,y,0,0,0,0); //à améliorer (en prenant en compte le sonar ou en estimant mieux la distance ?)
	printf("dropObject sent MSG_OBSTACLE at x=%d y=%d",x,y);
}

void takeObject(){
/*Fonction pour prendre un objet mobile
 *Makes a uturn take the object make a second uturn
 *Send a message to the sever*/
	rotate_to_angle(MAX_SPEED/6,180);
	servo_arm_down();
	int y=(Y+200*cos(TETA*PI/180))/50;
	int x=(X+200*sin(TETA*PI/180))/50;
	//send("MSG_OBSTACLE",x,y,0,0,0,1); //à améliorer (en prenant en compte le sonar ou en estimant mieux la distance ?)
	printf("takeObjet sent MSG_OBSTACLE at x=%d y=%d",x,y);
	rotate_to_angle(MAX_SPEED/6,-180);
}

int isMovableObstacle(){
/*Fonction pour savoir si l'object devant est un obstacle mobile ou non
 A faire : enregistrer la position de l'objet si ce n'est pas un objet mobile*/
	int a;
	rotate_to_angle(MAX_SPEED/8,-15);
	Sleep(100);
	a=red_obstacle();
	Sleep(100);
	rotate_to_angle(MAX_SPEED/8, 15);
	return(a);
}

int direction(){
	return (floor(TETA+45)/90)%4);
}
int checkForward(){
	int gauche=0;
	int face=0;
	int droite=0;
	int x;
	int y;
	int delta_x;
	int delta_y;
	char type = -1;
	//stop_sweep(); //sweep_thread
	go_to_distance_sweep_regular_braking_new(MAX_SPEED/8, 50, 40, 30);
	sleep(50);
	int dir = direction();
	switch(dir)
  {
   case 0: delta_x=1;
	 	break;
   case 1: delta_y=-1;
	 	break;
   case 2: delta_x=-1;
	 	break;
   case 3: delta_y=1;
   default: ;
  }
	face = get_sonar_distance();
	if (face<100){
		if(type<0){
			if (isMovableObstacle){
				type = 100;
			}
			else{
				type = 1;
			}
			setMapData(x,y,type);
		}
	}
	else{
			setMapData(x,y,0);
	}
	absolute_servo_sonar(-25);
	gauche = get_sonar_distance();
	if (gauche<100){
		if(type<0){
			if (isMovableObstacle){
				type = 100;
			}
			else{
				type = 1;
			}
			setMapData(x,y,type);
		}
	}
	else{
			setMapData(x-x_delta,y-y_delta,0);
	}
	absolute_servo_sonar(50);
	droite = get_sonar_distance();
	if (droite<100){
		if(type<0){
			if (isMovableObstacle){
				type = 100;
			}
			else{
				type = 1;
			}
			setMapData(x,y,type);
		}
	}
	else{
			setMapData(x+x_delta,y+y_delta,0);
	}
	absolute_servo_sonar(-25);
	go_to_angle(-180);
	int move_x = (X/50)%3;
	int move_y = (Y/50)%3;
	go_to_distance_sweep_regular_braking_new(MAX_SPEED/8,max(move_x,move_y)*10,40,30);
}
