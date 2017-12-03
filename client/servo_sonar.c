#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

#define HSONAR 120 // hauteur sonar en mm
#define DEPASSTSONAR 40 // hauteur sonar en mm
#ifndef PI
#define PI 3.14159265
#endif

#include "motors_servo.h"
#include "sensor_sonar.h"

int distance;
int distanceProj;
int i;
int j;
int k;
int distObstacle;
int nbTest;
char answer;
int minimum;
int angleMin;

int main( void ){
//to test each function, we need the main
	
    initMotorServo();
    initSensorSonar();
	
	servo_arm_up();
	
	printf("Initializing..");
	FILE * fichier = NULL;
	fichier = fopen("testSonarAngle","a");
	
	fprintf(fichier,"Test - angle rotation du bras : distance trouvée par le capteur\n");
	fprintf(fichier,"Les résultats sont affichés de la manière suivante : angle bras:valeur détectée\n");
	answer='y';
	
	while(answer!='n'){
		
		fprintf(fichier,"Distance de l'obstacle (en mm) : ");
		printf("Distance de l'obstacle (en mm) : ");
		scanf("%d",&distObstacle);
		fprintf(fichier,"%d\n", distObstacle);
		
		i=0;
		minimum=1000;
		angleMin=0;
		while (i<60){
			j=0;
			while(j<3){
				servo_sonar(i);
				distance = get_sonar_distance();
				if(distance<minimum){
					minimum = distance;
					angleMin = i;
				}
				fprintf(fichier,"%d : %d\n", i,distance);
				j=j+1;
			}
			i=i+5;
		}
		distanceProj = (int) pow((pow(minimum,2)-pow(HSONAR-DEPASSTSONAR*sin(angleMin*(2*PI)/360),2)),0.5);
		printf("Distance trouvée avec projection : %d\n",distanceProj);
		printf("Distance trouvée sans projection : %d\n",minimum);
		fprintf(fichier,"Distance trouvée : %d\n", distanceProj);
		k=k+1;
		printf("Continue the test ? (y/n)\n");
		Sleep(100);
		scanf(" %c",&answer);
		
	}
	
	servo_sonar(0);
	
	fclose(fichier);

}
