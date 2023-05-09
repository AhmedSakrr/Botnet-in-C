#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>



#include "utils_v2.h"
#include "header.h"

#define BACKLOG 5
#define BUFFER_SIZE 5000
#define SERVER_PORT 5000

 
char* executeCommand(char* cmd) {

	FILE * f =popen(cmd,"r");
    char result[24];
    char* buf = NULL;
    while (fgets(result, sizeof(result), f) != NULL){

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
	int option = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
	sbind(serverPort, sockfd);
	slisten(sockfd, BACKLOG);
	return sockfd;
}

int main(int argc, char** argv){

	int sockfd;

	if (argc > 1)
	{
		int port = atoi(argv[1]);
		sockfd = initSocketServer(port);
		printf("Le serveur tourne sur le port : %i\n", port);
	}else
	{
		sockfd = initSocketServer(SERVER_PORT);
		printf("Le serveur tourne sur le port : %i\n", SERVER_PORT);
	}	
	
	char* command;
	int newsockfd = saccept(sockfd);

	while(1){

		int nbCharRd = sread(newsockfd, &command, sizeof(command));
		swrite(1, &command, nbCharRd);

	    /* Ecris la valeur du client */
		//executeCommand(command);
		
		//swrite(newsockfd, result, strlen(result));	
	}
	sclose(newsockfd);
	sclose(sockfd);
}



