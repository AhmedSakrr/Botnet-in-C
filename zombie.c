#include <string.h>
#include <errno.h>
#include <libgen.h>
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
#define BUFFER_SIZE 500

 
char* executeCommand(char* cmd) {

	FILE * f =popen(cmd,"r");

    char result[24]={0x0};
    char* buf = NULL;
    while (fgets(result, sizeof(result), f) !=NULL){

        int taille = strlen(result);

        if (buf == NULL) {
	         // Première allocation de la chaîne s
	         buf = (char*) malloc((taille+1) * sizeof(char));
	         if (buf == NULL) return NULL;
	         // Copie des caractères du buffer dans s
	         strcpy(buf,result);
	      } else {
	         // Réallocation de la chaîne s
	         buf = (char*) realloc(buf, (strlen(buf)+taille+1) * sizeof(char));
	         if (buf == NULL) return NULL;
	         // Concaténation des caractères du buffer à la fin de s
	         strcat(buf,result);
	      }
    }

    pclose(f);
    return buf;
}


int initSocketServer(int serverPort)
{
  int sockfd = ssocket();

  /* no socket error */

  // setsockopt -> to avoid Address Already in Use
	int option = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));

  sbind(serverPort, sockfd);

  /* no bind error */
  slisten(sockfd, BACKLOG);

  /* no listen error */
  return sockfd;
}

int main(int argc, char** argv){

	int sockfd;
	int port;

	if (argc > 1)
	{
		port = atoi(argv[1]);
		sockfd = initSocketServer(port);
		printf("Le serveur tourne sur le port : %i\n", port);
	}
	else
	{
		// Select a random port from the list of available ports
		srand(time(NULL));
		int index = rand() % NUM_PORTS;
		port = PORTS[index];
		sockfd = initSocketServer(port);
		printf("Le serveur tourne sur le port : %i\n", port);
	}	
	
	char command[BUFFER_SIZE];

	/* Ecoute un client */
	int newsockfd = saccept(sockfd);

	while(1){
		
		/* Lit la valeur du client */
		sread(newsockfd, &command, sizeof(int));
		
	    /* Ecris la valeur du client */
		char* result = executeCommand(command);

		swrite(newsockfd, result, strlen(result));	

	}
	/* Ferme la connection client */
	sclose(newsockfd);
	/* Ferme le socket */
	sclose(sockfd);
}