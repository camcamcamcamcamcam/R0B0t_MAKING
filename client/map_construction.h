#ifndef R0B0T_MAKING_MAP_CONSTRUCTION_H
#define R0B0T_MAKING_MAP_CONSTRUCTION_H

#define map_x 80
#define map_y 80

extern unsigned char map[map_x][map_y];

void sendMapDone();
/* send all the pixels that are obstacles(both moveable and non-moveable) and also send at the end a map-done message */

void setMapData(int x, int y, unsigned char typeObject);
// set the correct type to the specified pixel (x,y) in the map variable.

unsigned char getMapData(int x, int y);
// get the correct type of the specified pixel

#define NO_OBSTACLE 0
#define NON_MOVING_OBSTACLE 1
#define MOVING_OBSTACLE 100
/*
 * the map is made of :
 *   0 if there is nothing
 *   1 if there is a non-moving obstacle
 *   100 if there is a moving obstacle
 *   200 if the value is unknown
 */
#endif //R0B0T_MAKING_MAP_CONSTRUCTION_H
