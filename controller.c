/**
 * AUTHORS : 
 * NZEYIMANA Yvan
 * OVERWEG Jef
 * VAN MOER Bryan
*/
#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "utils_v2.h"
#include "header.h"
#include "network.h"

#define BUFFER_SIZE 5000
#define MAX_ARGUMENTS 20
#define MAX_LENGTH 16
#define PHYSICAL_SIZE 10


volatile sig_atomic_t signalCount = 0;

pid_t childPid;


void handleSIGINT(int signal) { 
    signalCount = 1;
    skill(childPid, SIGINT);  
    exit(0);
}

int connectToZombies(char* serverIP, int serverPort);

void listenToTheZombies(void* ptrArray, void* logicalSize);

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Usage: ./controller ip1 ip2 ... ipN\n");
        return 1;
    }

    /* Array of file descriptor*/
    int* arrayFd = NULL; 
    int logicalSize = 0;  
    int physicalSize = PHYSICAL_SIZE;

    char command[BUFFER_SIZE];
    char servers[MAX_ARGUMENTS][MAX_LENGTH];
    int nbServers = argc - 1;
    
    arrayFd = (int*)smalloc(PHYSICAL_SIZE * sizeof(int));

    /* Copy args in an array */
    for (int i = 1; i < nbServers + 1; i++) {
        strncpy(servers[i - 1], argv[i], MAX_LENGTH - 1);
        servers[i - 1][MAX_LENGTH - 1] = '\0';
    }

    /* Connect to all the zombies and store their file descriptor in the array */
    for (int i = 0; i < nbServers; i++) {
        for(int j = 0; j < NUM_PORTS; j++) {

            int sockfd = connectToZombies(servers[i], PORTS[j]);

            /* Check if the connection succeed */
            if(sockfd != 0){
                
                /* Realloc the memory if it's necessary */
                if (logicalSize >= physicalSize) {
                    physicalSize += PHYSICAL_SIZE; 
                    int* newArray = (int*)realloc(arrayFd, physicalSize * sizeof(int));
                    if (newArray == NULL) {
                        printf("Erreur realloc.\n");
                        free(arrayFd);
                        return 0;
                    }
                    arrayFd = newArray;
                }

                /* Add file descriptor to the array */
                arrayFd[logicalSize] = sockfd;
                logicalSize++;
            }        
        }
    }
    if(logicalSize == 0){
        printf("Aucune connection établie \n");
        return 0;
    }
    printf("Nombre de connections établies: %d\n", logicalSize);

    ssigaction(SIGINT, handleSIGINT);

    childPid = fork_and_run2(listenToTheZombies, arrayFd, &logicalSize);

    /* PROGRAMME PERE */
   if (childPid > 0) {
        printf("Entrez une commande :\n");
        while (!signalCount) {
            if (fgets(command, BUFFER_SIZE, stdin) == NULL) {
                printf("Le controller s'arrête\n");
                signalCount = 1; 
                break;
            }
            for (int i = 0; i < nbServers; ++i)
            {
                for(int i = 0 ; i < logicalSize ; i++){
                    swrite(arrayFd[i], &command, strlen(command));
                }
            }
        }
        skill(childPid, SIGINT); 
    }

    /* Free allocated memory */
    free(arrayFd);

    /* Close all sockets */  
    for(int i = 0 ; i < logicalSize ; i++){
        sclose(arrayFd[i]);
    }
    return 0;
}


int connectToZombies(char* serverIP, int serverPort) {

    int sockfd;
    sockfd = ssocket();

    if(ssconnect(serverIP, serverPort, sockfd) != -1){
         printf("Connecté au serveur %s sur le port %d\n", serverIP, serverPort);
         return sockfd;
    } else {
        /* Error connection server */
        return 0;
    }
}

/* PROGRAMME FILS */
void listenToTheZombies(void* ptrArray, void* logicalSize) {

    int* arrayFd = ptrArray;
    int lSize = *(int*) logicalSize;
    char msg[BUFFER_SIZE];

    struct pollfd fds[1024];
    bool fds_invalid[1024];
    int nbSockfd = 0;

    //int s = 0;
    //while(s<1){
        for (int i = 0; i < lSize; i++) {
            fds[nbSockfd].fd = arrayFd[i];
            fds[nbSockfd].events = POLLIN;
            nbSockfd++;
            fds_invalid[nbSockfd] = false;
        }
        //s++;
    //}


    while (!signalCount) {
        spoll(fds, nbSockfd, 0);

        // Check if there are no active connections
        bool allInvalid = true;
        for (int i = 0; i < nbSockfd; i++) {
            if (!fds_invalid[i]) {
                allInvalid = false;
                break;
            }
        }

        if (allInvalid) {
            skill(getppid(), SIGINT);  // Send signal to parent process to stop execution
            break;
        }

        for (int i = 0; i < nbSockfd; i++) {
            if (fds[i].revents & POLLIN && !fds_invalid[i]) {

                int nbCharRead = sread(fds[i].fd, msg, sizeof(msg));
                if (nbCharRead <= 0) {
                    fds_invalid[i] = true;
                    continue;
                }
                printf("\nMessage reçu :\n%s\n", msg);
                memset(msg, 0, sizeof(msg));
            }
        }
    }

    for (int i = 0; i < nbSockfd; i++) {
        if (!fds_invalid[i]) {
            sclose(fds[i].fd);
        }
    }
}