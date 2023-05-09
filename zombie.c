#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "utils_v2.h"

#define MAX_SOCKET_SIMULTANEOUS 10
#define PORT 5000
#define BUFFER_SIZE 100


int main(int argc, char** argv){

	int server_port = PORT;

	/* Creer le socket serveur */
	int sockfd = ssocket();
	int option = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
	sbind(PORT, sockfd);
	slisten(sockfd, MAX_SOCKET_SIMULTANEOUS);

	printf("Zombie tourne sur le port : %i \n",server_port);

	char command[BUFFER_SIZE];

	/* Ecoute un client */
	int newsockfd = saccept(sockfd);

	while(1){
		
		/* Lit la valeur du client */
		sread(newsockfd, &command, sizeof(int));
		
	    /* Ecris la valeur du client */
	    swrite(1, &command, strlen(command));
	}
	
	/* Ferme la connection client */
	sclose(newsockfd);
	/* Ferme le socket */
	sclose(sockfd);
	
	exit(0);
}



