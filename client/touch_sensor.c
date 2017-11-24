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



int isTouched(){

    uint8_t sn_touch;
    int value;

    if (ev3_search_sensor(LEGO_EV3_TOUCH, &sn_touch,0)){
        if ( !get_sensor_value0(sn_touch, &value )) {
            value = 0;
        }
        fflush( stdout );
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

    while (1){

        value = isTouched();
        if(value){
            printf("Touched\n");
        } else{
            printf("Not touched\n");
        }
        sleep(2);

    }

    ev3_uninit();
    printf( "*** ( EV3 ) Bye! ***\n" );

    return ( 0 );
}
