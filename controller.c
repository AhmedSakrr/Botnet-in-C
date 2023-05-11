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
#define SERVER_PORT 5000

char* server_ip;
int server_port;

int main(int argc, char** argv) {

    if(argc != 3) {
        printf("Argument invalide : ./client server_ip server_port\n");
        exit(EXIT_FAILURE);
    }

    server_ip = argv[1];
    server_port = atoi(argv[2]);
    char command[BUFFER_SIZE];
    //char result[BUFFER_SIZE];

    // Create socket
    int sockfd = ssocket();

    sconnect(server_ip, server_port, sockfd);
    printf("Controlleur tourne sur le port : %i \n",server_port);

    // Send characters to the server
    printf("Entrez un entier:\n");
    while (1) {
        fgets(command, sizeof(command), stdin);

        // Remove trailing newline character
        command[strcspn(command, "\n")] = 0;

        // Send the message to the server
        swrite(sockfd, &command, strlen(command));

        int s = sread(sockfd, &command, BUFFER_SIZE);

        swrite(1, &command, s);
    }

    // Close the socket
    sclose(sockfd);

    return 0;
}