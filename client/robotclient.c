

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/socket.h>
#include <math.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define SERV_ADDR  "5c:51:4f:29:bc:be"   //"benjiii bluetooth address 5c:51:4f:29:bc:be" //"dc:53:60:ad:61:90"    PROF"00:1a:7d:da:71:06" /* Whatever the address of the server is */
#define TEAM_ID     2                       /* Your team ID */

#define MSG_ACK     0
#define MSG_START    1
#define MSG_STOP   2
#define MSG_KICK    3
#define MSG_POSITION 4
#define MSG_MAPDATA     5
#define MSG_MAPDONE 6
#define MSG_OBSTACLE 7
#define Sleep( msec ) usleep(( msec ) * 1000 )
#define TAG "CLIENT : "
#define DEBUG 1

char server_said_stop = 0;

int read_from_server (int sock, char *buffer, size_t maxSize);

void debug (const char *fmt, ...) {
    va_list argp;

    va_start (argp, fmt);

    vprintf (fmt, argp);

    va_end (argp);
}


int s;
int status;

uint16_t msgId = 0;

void initClient() {
    if (DEBUG) printf("CLIENT : initializing\n");
    struct sockaddr_rc addr = { 0 };

    /* allocate a socket */
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* set the connection parameters (who to connect to) */
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba (SERV_ADDR, &addr.rc_bdaddr);

    /* connect to server */
    if (DEBUG) printf("CLIENT : Trying to connect to the server\n");
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    if( status == 0 ) {
        printf("wait for START message\n");
        char string[58];
        string[4] = MSG_STOP; //to be sure it is not initialized to MSG_START

        //wait for the start message
        while (string[4] != MSG_START) read_from_server(s, string, 9);

        printf("Received start message!\n");

    } else {
        fprintf (stderr, "Failed to connect to server...\n");
        sleep (2);
        exit (EXIT_FAILURE);
    }
}

int read_from_server (int sock, char *buffer, size_t maxSize) {
    int bytes_read = read (sock, buffer, maxSize);

    if (bytes_read <= 0) {
        fprintf (stderr, "Server unexpectedly closed connection...\n");
        close (s);
        exit (EXIT_FAILURE);
    }

    printf ("[DEBUG] received %d bytes\n", bytes_read);

    return bytes_read;
}

char size_message(char msg_type) {
    switch (msg_type) {
        /*case MSG_ACK :    only sent by the server
            return 8;
        case MSG_START:
            return 5;
        case MSG_STOP:
            return 5;
        case MSG_KICK:
            return 6;*/
        case MSG_POSITION:
            return 9;
        case MSG_MAPDATA:
            return 12;
        case MSG_MAPDONE:
            return 5;
        case MSG_OBSTACLE:
            return 10;
    }
    return (-1);
}

char sendMessage(char msg_type, int x, int y, int R, int G, int B, char act) {
    /*
     * int x, int y, int R, int G, int B, char act are optional, they should be put to 0 when unnecessary
     * x and y are for position, R, G, B are colors between 0 and 255,
     * act is 0 if the robot dropped an obstable, 1 if it picked it
     */
    if( status == 0 ) {
        char string[58];
        *((uint16_t *) string) = msgId++;
        string[2] = TEAM_ID;
        string[3] = 0xFF;
        string[4] = msg_type;
        switch (msg_type) {
            case MSG_POSITION: // to test
                string[5] = x;          /* x */
                string[6] = 0x00;
                string[7] = y;              /* y */
                string[8] = 0x00;
                break;
            case MSG_MAPDATA:
                string[5] = x;          /* x */
                string[6] = 0x00;
                string[7] = y;          /* y */
                string[8] = 0x00;
                string[9] = R;
                string[10] = G;
                string[11] = B;
                break;
            case MSG_OBSTACLE:
                string[5] = act;
                string[6] = x;
                string[7] = 0x00;
                string[8] = y;
                string[9] = 0x00;
        }

        write(s, string, size_message(msg_type));
        return (1);
    }
    return (0);
}

void receiveMessageServer(){
  if( status == 0 ) {
      //printf("wait for STOP or KICK message\n");
      char string[58];
      char type;
      string[4] = MSG_START; //to be sure it is not initialized to STOP
      //wait for the stop or kick message
      while(1){
          //Wait for stop message
          read_from_server (s, string, 58);
          type = string[4];
          if (type == MSG_STOP || type ==  MSG_KICK){
              server_said_stop = 1;
              if (type == MSG_STOP) printf("*\n**\n***\n**** THE GAME HAS STOPED\n***\n**\n*\n");
              else printf("*\n**\n***\n**** WE WERE KICKED OUT OF THE GAME :(\n***\n**\n*\n");
              return;
          }
      }

  } else {
      fprintf (stderr, "Failed to connect to server...\n");
      sleep (2);
      exit (EXIT_FAILURE);
  }
}


void robot2() {
    /*
     * second test to see if function sendMessage is working
     */
    int i;
    for (i=0; i<30; i++){
        sendMessage(MSG_POSITION,i,i,0,0,0,0);
        Sleep(1000);
    }
    int x1 = rand() % 30;
    int y1= rand() % 30;
    for (i=0; i<30; i++) {
        sendMessage(MSG_MAPDATA, x1 + i, y1 + i,0,0,0,0);
        Sleep(1000);
    }
    sendMessage(MSG_MAPDONE,0,0,0,0,0,0);
}

void robot () {
    char string[58];
    char type;
    int x1, x2, y1, y2;
    printf ("I'm navigating...\n");

    srand(time(NULL));
    /* Send 30 POSITION messages, a BALL message, 1 position message, then a NEXT message */
    int i, j;
    for (i=0; i<30; i++){
        *((uint16_t *) string) = msgId++;
        string[2] = TEAM_ID;
        string[3] = 0xFF;
        string[4] = MSG_POSITION;
        string[5] = i;          /* x */
        string[6] = 0x00;
        string[7] = i;              /* y */
        string[8] = 0x00;
        printf("%d \n", sizeof(msgId));
        write(s, string, 9);
        Sleep( 1000 );
    }

    printf ("I'm sending my map...\n");

    /* MAP data is in the form MAPDATA | X  X |Y  Y | R | G | B */

    /* Create 1 square green obstacle and 1 round red obstacle*/
    /* Send only the outline */
    x1 = rand() % 30;
    y1= rand() % 30;

    for (i=x1; i<x1+10; i++){
        *((uint16_t *) string) = msgId++;
        string[2] = TEAM_ID;
        string[3] = 0xFF;
        string[4] = MSG_MAPDATA;
        string[5] = i;          /* x */
        string[6] = 0x00;
        string[7] = y1;             /* y */
        string[8]= 0x00;
        string[9]= 0;
        string[10]=254;
        string[11]= 0;
        write(s, string, 12);
        Sleep( 100 );
    }
    for (i=x1; i<x1+10; i++){
        *((uint16_t *) string) = msgId++;
        string[2] = TEAM_ID;
        string[3] = 0xFF;
        string[4] = MSG_MAPDATA;
        string[5] = i;          /* x */
        string[6] = 0x00;
        string[7] = y1+10;          /* y */
        string[8]= 0x00;
        string[9]= 0;
        string[10]=254;
        string[11]= 0;
        write(s, string, 12);
        Sleep( 100 );
    }
    for (j=y1; j<y1+10; j++){
        *((uint16_t *) string) = msgId++;
        string[2] = TEAM_ID;
        string[3] = 0xFF;
        string[4] = MSG_MAPDATA;
        string[5] = x1;          /* x */
        string[6] = 0x00;
        string[7] = j;              /* y */
        string[8]= 0x00;
        string[9]= 0;
        string[10]=254;
        string[11]= 0;
        write(s, string, 12);
        Sleep( 100 );
    }

    for (j=y1; j<y1+10; j++){
        *((uint16_t *) string) = msgId++;
        string[2] = TEAM_ID;
        string[3] = 0xFF;
        string[4] = MSG_MAPDATA;
        string[5] = x1+10;          /* x */
        string[6] = 0x00;
        string[7] = j;              /* y */
        string[8]= 0x00;
        string[9]= 0;
        string[10]=254;
        string[11]= 0;
        write(s, string, 12);
        Sleep( 100 );
    }

    x2 = 15 +rand() % 20;
    y2= 15 + rand() % 20;


    for (i=x2-15; i<x2+15; i++){
        for (j=y2-15; j<y2+15; j++){
            if (sqrt((i-x2)*(i-x2) + (j-y2)*(j-y2)) < 15 && sqrt((i-x2)*(i-x2) + (j-y2)*(j-y2))>14){
                *((uint16_t *) string) = msgId++;
                string[2] = TEAM_ID;
                string[3] = 0xFF;
                string[4] = MSG_MAPDATA;
                string[5] = i;          /* x */
                string[6] = 0x00;
                string[7] = j;          /* y */
                string[8]= 0x00;
                string[9]= 254;
                string[10]=0;
                string[11]= 0;
                write(s, string, 12);
                Sleep( 100 );
            }
        }
    }
    printf("Done sending map\n");
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_MAPDONE;
    write(s, string, 5);

    printf("I'm waiting for the stop message\n");
    while(1){
        //Wait for stop message
        read_from_server (s, string, 58);
        type = string[4];
        if (type ==MSG_STOP){
            return;
        }
    }
}




/*
int main(void) {

    //struct sockaddr_rc addr = { 0 };

    // allocate a socket
    //s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    //addr.rc_family = AF_BLUETOOTH;
    //addr.rc_channel = (uint8_t) 1;
    //str2ba (SERV_ADDR, &addr.rc_bdaddr);

    // connect to server
    //status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    init_client();

    // if connected
    //if( status == 0 ) {
        //if (DEBUG) printf("CLIENT : We are connected to the server");
        //char string[58];

        // Wait for START message
        //printf("wait for START message\n");
        //read_from_server (s, string, 9);
        //if (string[4] == MSG_START) {
        //    printf ("Received start message!\n");

        //}
        //robot();
        robot2();
        close (s);

        sleep (5);

    //} else {
       // fprintf (stderr, "Failed to connect to server...\n");
      //  sleep (2);
     //   exit (EXIT_FAILURE);
    //}

    close(s);
    return 0;
}
*/
