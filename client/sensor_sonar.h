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

uint8_t sn_sonar;
int bufferSonar[5]; // contains the ten last values of the sonar value.
int indexSonar; // index of the next element to write in the buffer

void initSensorSonar();
/*need to be started at the beginning
 Allows to use the sonar sensor */
int get_sonar_distance();
/* return the distance found by the sensor in mm.
*/
int getMinBufferSonar();
/*
return the minimum value of the sonar sensor over the last measures done by the sonar.
*/