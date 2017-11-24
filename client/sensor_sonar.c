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


void initSensorSonar(){
	// initialise the sonar sensor
    if ( ev3_init() == -1 ) return ( 1 );
	ev3_sensor_init();
	ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0);

}

int get_sonar_distance(){
    // return the distance in mm got by the sensor
	int value;
    get_sensor_value0(sn_sonar,&value);
	fflush( stdout );
	printf("value is %d",value);
	if(value>=750){
		return 750;
	}
	else{
		return value;
	}
}


int main( void ){

	int distance;
    initSensorSonar();
    while(1){
		distance = get_sonar_distance();
		printf("Distance captée : %d \n",distance);
		Sleep(1000);
	}

}
