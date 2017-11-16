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

void go_straight(int speed, int duration){
	run_motor(66, speed, duration);
	run_motor(68, speed, duration);
}

int get_motor_position(int port){
	uint8_t sn;
	int position;
	if( ev3_search_tacho_plugged_in(port,0,&sn,0) ){
		get_tacho_position(sn,&position);
	}
	printf("position : %d \n",position);
	return position;
}

void run_motor_pos(int port, int position){
	uint8_t sn;
	if ( ev3_search_tacho_plugged_in(port,0,&sn,0) ){
		position = get_tacho_position(sn, &position);
	}
	set_tacho_position(sn,position+10000);
	set_tacho_command_inx(sn, TACHO_RUN_TIMED);
}

void run_motor(int port, int speed, int duration){
	// Port detected : 66 left and 68 right
	uint8_t sn;
	int eff_speed;
	int polarity;
	if ( ev3_search_tacho_plugged_in(port,0, &sn, 0 )) {
		int max_speed;
		get_tacho_max_speed( sn, &max_speed );
		if(speed<0){
			polarity = set_tacho_polarity_inx(sn,TACHO_INVERSED);
			speed = -speed;
		}
		else{
			polarity = set_tacho_polarity_inx(sn,TACHO_NORMAL);
		}
		if(speed > max_speed){
			eff_speed = max_speed;
		}
		else{
			eff_speed = speed;
		}
		
		printf("Effective speed : %d \n",eff_speed);
		set_tacho_speed_sp( sn, eff_speed);
		set_tacho_time_sp( sn, duration );
		set_tacho_command_inx(sn, TACHO_RUN_TIMED);
	}
}

int main( void )
{
	int i;
	uint8_t sn;
	FLAGS_T state;
	uint8_t sn_touch;
	uint8_t sn_color;
	uint8_t sn_compass;
	uint8_t sn_sonar;
	uint8_t sn_mag;
	char s[ 256 ];
	int val;
	float value;
	uint32_t n, ii;
#ifndef __ARM_ARCH_4T__
	/* Disable auto-detection of the brick (you have to set the correct address below) */
	ev3_brick_addr = "192.168.0.204";

#endif
	if ( ev3_init() == -1 ) return ( 1 );

#ifndef __ARM_ARCH_4T__
	printf( "The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr );

#else
	printf( "Waiting tacho is plugged...\n" );

#endif
	while ( ev3_tacho_init() < 1 ) Sleep( 1000 );

	printf( "*** ( EV3 ) Hello! ***\n" );

	printf( "Found tacho motors:\n" );
	for ( i = 0; i < DESC_LIMIT; i++ ) {
		if ( ev3_tacho[ i ].type_inx != TACHO_TYPE__NONE_ ) {
			printf( "  type = %s\n", ev3_tacho_type( ev3_tacho[ i ].type_inx ));
			printf( "  port = %s\n", ev3_tacho_port_name( i, s ));
			printf("  port = %d %d\n", ev3_tacho_desc_port(i), ev3_tacho_desc_extport(i));
		}
	}
	// Run motors in order from port A to D
	// Port detected : 66 left and 68 right
	int duration=4000;
	int speed=1000;
	/*
	go_straight(speed, duration);
	int position = 1;
	while(position>0){
		position = get_motor_position(66);
	}
	*/
	int position;
	position = get_motor_position(66);
	run_motor_pos(66,1000);
	while(position>0){
		position = get_motor_position(66);
		sleep(1);
	}
	printf( "*** ( EV3 ) Bye! ***\n" );
	
	return ( 0 );
}
