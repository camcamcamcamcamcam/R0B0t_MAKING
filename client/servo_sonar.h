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

int getDistance(int angle);
/* get the distance found in the specified angle. */

int getDistance_weighted(int angle);
/* get the distance found in the specified angle. */

int getMinDistance(int amplitudeAngle, int precisionAngle);
/* get the min distance in the nearby. */

int getMinBufferSonar();
/*
return the minimum value of the sonar sensor over the last measures done by the sonar.
*/
void clearBuffer();