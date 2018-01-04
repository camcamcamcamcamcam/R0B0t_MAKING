#include "map_construction.h"
#include <stdio.h>
#include "robotclient.h"

#define map_x 80
#define map_y 80

/*
 * the map is made of :
 *   0 if there is nothing
 *   1 if there is a non-moving obstacle
 *   100 if there is a moving obstacle
 *   200 if the value is unknown
 */


static unsigned char map[map_x][map_y] = { [0 ... map_x-1][0 ... map_y-1] = 200};


void sendMapDone(){
    for (int i=0; i < map_x; i++){
        for (int j=0; j < map_y; j++){
            switch (map[i][j]) {
                case NON_MOVING_OBSTACLE :
                    sendMessage(MSG_MAPDATA, i, j, 255, 255, 255, 0);
                    break;
                case MOVING_OBSTACLE :
                    sendMessage(MSG_MAPDATA, i, j, 255, 0, 0, 0);
                    break;
                default:    //nothing there
                    break;
            }
        }
    }
    sendMessage(MSG_MAPDONE, 0, 0, 0, 0, 0, 0);
}

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