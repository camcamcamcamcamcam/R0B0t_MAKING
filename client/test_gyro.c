/*
     ____    __   ___   ___  _____  ____
    | |__) //\ \ | |_) | |_)  | |  | |_
    |_| \  \\_\/ |_|_) |_|_) _|_|_ |_|__
                                   
 *//**
 *  \file  test_gyro.c
 *  \brief  Concerning the gyro sensor.
 */

#include <string.h>
#include <stdio.h>
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


int main( void )
{
	char s[ 256 ];
	int val;
	uint32_t n, i, ii;
	uint8_t sn_gyro;

	printf( "Waiting the EV3 brick online...\n" );
	if ( ev3_init() < 1 ) return ( 1 );

	printf( "*** ( EV3 ) Hello! ***\n" );
	ev3_sensor_init();

	printf( "Found sensors:\n" );
	for ( i = 0; i < DESC_LIMIT; i++ ) {
		if ( ev3_sensor[ i ].type_inx != SENSOR_TYPE__NONE_ ) {
			printf( "  type = %s\n", ev3_sensor_type( ev3_sensor[ i ].type_inx ));
			printf( "  port = %s\n", ev3_sensor_port_name( i, s ));
			if ( get_sensor_mode( i, s, sizeof( s ))) {
				printf( "  mode = %s\n", s );
			}
			if ( get_sensor_num_values( i, &n )) {
				for ( ii = 0; ii < n; ii++ ) {
					if ( get_sensor_value( ii, i, &val )) {
						printf( "  value%d = %d\n", ii, val );
					}
				}
			}
		}
	}
	if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyro, 0 )) {
		printf( "GYRO sensor is found\n" );
		set_sensor_mode( sn_gyro, "GYRO-G&A" );
		for ( ; ; ) {
			if ( !get_sensor_value( 0, sn_gyro, &val ) ) {
				val = 0;
			}
			printf( "valeur angle gyro (%d)\n", val );
                        if ( !get_sensor_value( 1, sn_gyro, &val ) ) {
                                val = 0;
                        }
			printf( "valeur rotationnal speed gyro (%d)\n", val );
			Sleep( 200 );
		}
	} else {
		printf( "GYRO sensor is NOT found\n" );
	}
	ev3_uninit();
	printf( "\n*** ( EV3 ) Bye! ***\n" );

	return ( 0 );
}
