#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
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

#include "sensor_sonar.h"

uint8_t sn_sonar;
int bufferSonar[5]; // contains the ten last values of the sonar value.
int indexSonar; // index of the next element to write in the buffer


void initSensorSonar(){
	// initialise the sonar sensor
    if ( ev3_init() == -1 ) return ( 1 );
	ev3_sensor_init();
	ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0);
}

int get_sonar_distance(){
    // return the distance in mm got by the sensor
	float value;
    get_sensor_value0(sn_sonar,&value);
	fflush( stdout );
	printf("Sonar sensor : %f\n",value);
	if(value>=750){
		bufferSonar[indexSonar] = 750;
		indexSonar = (indexSonar+1)%5;
		return 750;
	}
	else{
		bufferSonar[indexSonar] = (int) value;
		indexSonar = (indexSonar+1)%5;
		return (int) value;
	}
}

int getMinBufferSonar(){
	int i=1;
	int minDistance=bufferSonar[0];
	while(i<5){
		if(bufferSonar[i]<minDistance){
			minDistance = bufferSonar[i];
		}
		i = i+1;
	}
	return minDistance;
}
/*
int main( void ){

	int distance;
    initSensorSonar();
    while(1){
		distance = get_sonar_distance();
		printf("Distance captée : %d \n",distance);
		Sleep(1000);
	}

}
*/
