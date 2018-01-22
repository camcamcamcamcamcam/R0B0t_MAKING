
#ifndef R0B0T_MAKING_ROBOTCLIENT_H
#define R0B0T_MAKING_ROBOTCLIENT_H

char sendMessage(char msg_type, int x, int y, int R, int G, int B, char act);
    /*
     * int x, int y, int R, int G, int B, char act are optional, they should be put to 0 when unnecessary
     * x and y are for position, R, G, B are colors between 0 and 255,
     * act is 0 if the robot dropped an obstable, 1 if it picked it
     */


void initClient();
void receiveMessageServer();
char size_message(char msg_type);
int read_from_server (int sock, char *buffer, size_t maxSize);
void robot2();
void robot();
char sendMessage(char msg_type, int x, int y, int R, int G, int B, char act);
#define MSG_ACK     0
#define MSG_START    1
#define MSG_STOP   2
#define MSG_KICK    3
#define MSG_POSITION 4
#define MSG_MAPDATA     5
#define MSG_MAPDONE 6
#define MSG_OBSTACLE 7

extern char server_said_stop;


#endif //R0B0T_MAKING_ROBOTCLIENT_H
