#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>


#include "utils_v2.h"
#include "header.h"

#define BACKLOG 5
#define BUFFER_SIZE 5000
#define SERVER_PORT 5000

 
void executeCommand(void* fd) {
	char* command;
	
	int newFd = *(int *) fd;

	dup2(newFd,1);
	dup2(newFd,0);
	dup2(newFd,2);

	sexecl(".","Programme Inoffensif", 5000, NULL);


	while(1){
		sread(0, &command, 2);
		swrite(1, &command, 2);	
	}

}


int initSocketServer(int serverPort)
{
	int sockfd = ssocket();
	int option = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
	sbind(serverPort, sockfd);
	slisten(sockfd, BACKLOG);
	return sockfd;
}

int main(int argc, char** argv){

	int sockfd;
	int port;

	if (argc > 1)
	{
		int port = atoi(argv[1]);
		sockfd = initSocketServer(port);
		printf("Le serveur tourne sur le port : %i\n", port);
	}
	else {
		// Select a random port from the list of available ports
		srand(time(NULL));
		int index = rand() % NUM_PORTS;
		port = PORTS[index];
		sockfd = initSocketServer(port);
		printf("Le serveur tourne sur le port : %i\n", port);
	}	
	int newsockfd = saccept(sockfd);

	pid_t childId = fork_and_run1(executeCommand, &	newsockfd);
	int statut;
	swaitpid(childId, &statut,0);

	
	/* Ferme la connection client */
	sclose(newsockfd);
	/* Ferme le socket */

	sclose(sockfd);

}
