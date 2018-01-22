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

uint8_t sn_touch;
/*
@desc : initialize the touch sensor. The function has to be called once before starting using the touch sensor.
@param : /
@author : Benjamin Castellan
@return : void
*/
void initTouchSensor(){
    //initialize the touch sensor
    if ( ev3_init() == -1 ) return ( 1 );
    ev3_sensor_init();
    ev3_search_sensor(LEGO_EV3_TOUCH, &sn_touch,0);

}

/*
@desc : check if the sensor is being pushed
@param : /
@author : Benjamin Castellan
@return : 1 if touched, 0 else
*/
int isTouched(){
    //return true if the sensor is pushed, false if not
    int value;
    if ( !get_sensor_value0(sn_touch, &value )) {
        value = 0;
    }
    fflush( stdout );
    return value;
}

/*
@desc : test if the touchSensor is working properly
@param : /
@author : Benjamin Castellan
@return : 0
*/
int main( void )
{

    initTouchSensor();
    int value;
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
