#include "map_construction.h"
#include <stdio.h>
#include "robotclient.h"



unsigned char map[map_x][map_y] = { [0 ... map_x-1][0 ... map_y-1] = 200};
/*
 * the map is made of :
 *   0 if there is nothing
 *   1 if there is a non-moving obstacle
 *   100 if there is a moving obstacle
 *   200 if the value is unknown
 */

/*
@desc : send all the pixels that are obstacles(both moveable and non-moveable) and also send at the end a map-done message
@param : /
@author : Benjamin Castellan
@return : void
*/
void sendMapDone(){
  int obstable = 0;
    for (int i=0; i < map_x; i++){
        for (int j=0; j < map_y; j++){
            switch (map[i][j]) {
                case NON_MOVING_OBSTACLE :
                    sendMessage(MSG_MAPDATA, i, j, 0, 0, 0, 0);
                    obstable++;
                    break;
                case MOVING_OBSTACLE :
                    sendMessage(MSG_MAPDATA, i, j, 255, 0, 0, 0);
                    obstable++;
                    break;
                default:    //nothing there
                    break;
            }
        }
    }
    printf("There were %d obstacle\n", obstable);
    sendMessage(MSG_MAPDONE, 0, 0, 0, 0, 0, 0);
}

/*
@desc : set the correct type to the specified pixel (x,y) in the map variable
@param : 
	* char x : x-axis index in the map (X/50), where X is in mm
	* char y : y-axis index in the map (Y/50), where Y is in mm
	* unsigned char typeObject : char defining the nature of the obstacle (0, 1, 100, 200)
@author : Benjamin Castellan
@return : void
*/
void setMapData(char x, char y, unsigned char typeObject){
    map[x][y] = typeObject;
}


/*
int main(void){

    initClient();
    printf("client has been initialized");
    for (int i=0; i<10;i++){
        map[i][4] = 0;
        map[i][5] = 0;
        map[i][6] = 0;
        map[i][7] = 0;
    }
    map[40][40] = 100;

    sendMapDone();
    printf("map has been sent");
    return 1;
}
*/
