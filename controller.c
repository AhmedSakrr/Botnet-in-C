
#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "utils_v2.h"

#define BUFFER_SIZE 100
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
    }

    // Close the socket
    sclose(sockfd);

    return 0;
}



// int main(int argc, char** argv){

//     char command[BUFFER_SIZE];

//     int sockfd = ssocket();
// 	int option = 1;

//     int newSockFd = sconnect("127.0.0.1", PORT, sockfd);

//     while(1){

//         sread(newsockfd, &value, sizeof(int));

//         swrite(sockfd, &transaction, sizeof(transaction));

//         printf("\n");
//     }

    

//     sclose(sockfd);
// }




