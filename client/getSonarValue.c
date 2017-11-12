#include <stdio.h>
#include <stdlib.h>
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

float valueToDistance(float sensorValue){
	if(sensorValue>=744){
		return 95;
	}
	else{
		return 0.1*sensorValue;
	}
}

float getSonarDistance(){
	
	uint8_t sn_sonar;
	float value;
	
	if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
		//printf("SONAR found, reading sonar...\n");
		if ( !get_sensor_value0(sn_sonar, &value )) {
			value = 0;
		}
		//printf( "\r Distance detected in cm (%f) \n", valueToDistance(value));
		fflush( stdout );
		value = valueToDistance(value);
	}
	else{
		value = -1;
	}
	
	return value;
}

int main( void )
{
	
	float value;
	
	if ( ev3_init() == -1 ) return ( 1 );

	printf( "*** ( EV3 ) Hello! ***\n" );
	
//Run all sensors
	ev3_sensor_init();

	while (True){
		
		value = getSonarDistance();
		printf("Valeur détectée : %f cm",value)
		sleep(2);
		
	}
	
	fclose(fichier);

	ev3_uninit();
	printf( "*** ( EV3 ) Bye! ***\n" );

	return ( 0 );
}
