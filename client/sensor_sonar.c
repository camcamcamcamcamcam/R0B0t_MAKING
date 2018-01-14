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

/*
@desc : initialize the sensor sonar. The function has to be called once before starting using the sonar.
@param : /
@author : Samuel Pierre
@return : void
*/
void initSensorSonar(){
	// initialise the sonar sensor
    if ( ev3_init() == -1 ) return ( 1 );
	ev3_sensor_init();
	ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0);
}

/*
@desc : the function enables to get the value detected by the sonar sensor
@param : /
@author : Samuel Pierre
@return : (int) distance in mm detected by the sonar
*/
int get_sonar_distance(){
	float value;
    get_sensor_value0(sn_sonar,&value);
	fflush( stdout );
	return (int) value;
}
