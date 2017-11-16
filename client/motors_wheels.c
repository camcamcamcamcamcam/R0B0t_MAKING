#include "motors_wheels.h"


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

  if ( ev3_init() == -1 ) return ( 1 );
  while ( ev3_tacho_init() < 1 ) Sleep( 1000 );//do not remove this line, or the LEGO_EV3_M_MOTOR 1 will NOT be found
  int port=65;
	for (port=65; port<69; port++){
    if ( ev3_search_tacho_plugged_in(port,0, &sn, 0 )) {
    int max_speed;
    get_tacho_max_speed( sn, &max_speed );

    printf( "LEGO_EV3_M_MOTOR 1 is found, run for 5 sec...\n" );
    get_tacho_max_speed( sn, &max_speed );
    printf("  max speed = %d\n", max_speed );
    set_tacho_stop_action_inx( sn, TACHO_COAST );
    set_tacho_speed_sp( sn, max_speed * 2 / 3 );
    set_tacho_time_sp( sn, 5000 );
    set_tacho_ramp_up_sp( sn, 2000 );
    set_tacho_ramp_down_sp( sn, 2000 );
    set_tacho_command_inx( sn, TACHO_RUN_TIMED );
    /* Wait tacho stop */
    Sleep( 100 );
    do {
      get_tacho_state_flags( sn, &state );
    } while ( state );
    printf( "run to relative position...\n" );
    set_tacho_speed_sp( sn, max_speed / 2 );
    set_tacho_ramp_up_sp( sn, 0 );
    set_tacho_ramp_down_sp( sn, 0 );
    set_tacho_position_sp( sn, 90 );
    for ( i = 0; i < 8; i++ ) {
      set_tacho_command_inx( sn, TACHO_RUN_TO_REL_POS );
      Sleep( 500 );
    }

  } else {
    printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
  }
}
}
