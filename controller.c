#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "utils_v2.h"

#define BUFFER_SIZE 1000

int connectToServer(char* serverIP, int port) {

    int sockfd;
    sockfd = ssocket();
    sconnect(serverIP, port, sockfd);
    
    printf("Connected to server %s on port %d\n", serverIP, port);
    return sockfd;
}

int main(int argc, char** argv) {

    char* servers[] = {
        "127.0.0.1",
        // "192.168.0.100",
        // "10.0.0.50"
    };

    int ports[] = {
        5000, 
        5001,
        5002,
    };

    int portsLength   = sizeof(ports)    / sizeof(int);
    int serversLength = sizeof(servers)  / sizeof(char*);

    for (int i = 0; i < serversLength; i++) {
        for(int j = 0; j < portsLength; j++) {
            int sockfd = connectToServer(servers[i], ports[j]);
            sclose(sockfd);
        }
    }

    //-----------------------------------------------------------------s

    //int sockfd = initSocketClient(server_ip, SERVER_PORT);
    //printf("Controlleur tourne sur le port : %i \n",server_port);

    char command[BUFFER_SIZE];

    // // Send characters to the server
    // printf("Entrez une commande :\n");
    while (1) {
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        //swrite(sockfd, &command, strlen(command));

        // int s = sread(sockfd, &command, BUFFER_SIZE);

        // swrite(1, &command, s);
    }

    // Close the socket
    //sclose(sockfd);

    //-----------------------------------------------------------------


    return 0;
}
