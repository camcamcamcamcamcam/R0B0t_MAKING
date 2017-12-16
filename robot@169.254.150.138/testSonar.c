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


int main( void )
{
	uint8_t sn_sonar;
	char s[ 256 ];
	float value;
	
	if ( ev3_init() == -1 ) return ( 1 );

	while ( ev3_tacho_init() < 1 ) Sleep( 1000 );

	printf( "*** ( EV3 ) Hello! ***\n" );
	
//Run all sensors
	ev3_sensor_init();
	
	int expectedValue;
	FILE * fichier = NULL;
	fichier = fopen("testSonar.txt","a");
	fprintf(fichier,"# The file contains a kind of table expectedValue (in cm) : value from the sensor \n");
	
	printf("Value measured (in cm) ?");
	scanf("%d",&expectedValue);
	printf("\n");
	while (expectedValue!=0){
		
		if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
			printf("SONAR found, reading sonar...\n");
			if ( !get_sensor_value0(sn_sonar, &value )) {
				value = 0;
			}
			printf( "\r(%f) \n", value);
			printf("Value expected : %d \n", expectedValue);
			printf("Value measured : %f \n", value);
			fprintf(fichier, "%d : %f \n", expectedValue, value);
			fflush( stdout );
	    	}
		
		printf("Valeur mesurée (en cm) ? (0 to exit)");
		scanf("%d",&expectedValue);
		printf("\n");
	}
	
	fclose(fichier);

	ev3_uninit();
	printf( "*** ( EV3 ) Bye! ***\n" );

	return ( 0 );
}
