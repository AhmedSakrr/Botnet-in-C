#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include "header.h"
#include "utils_v2.h"

#define NUM_ZOMBIES 2

int zombies[NUM_ZOMBIES];
int zombie_pgid;

int used_ports[NUM_PORTS] = {0};

void launch_zombie(int index) {
    srand(time(NULL));
    int port_index, port;
    do {
        port_index = rand() % NUM_PORTS;
        port = PORTS[port_index];
    } while (used_ports[port_index]);
    used_ports[port_index] = 1;
    pid_t pid = fork();
    if (pid == 0) {
        char str_port[6];
        sprintf(str_port, "%d", port);
        execl("./zombie", "zombie", str_port, NULL);
        exit(0);
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else {
        zombies[index] = pid;
        if (index == 0) {
            zombie_pgid = pid;  
        }
        setpgid(pid, zombie_pgid);  
    }
}

void terminate_zombies() {
    skill(-zombie_pgid, SIGTERM);  // Send the SIGTERM signal to the entire process group
    int status;
    for (int i = 0; i < NUM_ZOMBIES; i++) {
        if (zombies[i] > 0) {
            waitpid(zombies[i], &status, 0);  // Wait for the zombie process to terminate
        }
    }
}

void handleSIGINT(int signal) {
    terminate_zombies();
    exit(0);
}

int main(int argc, char** argv) {
    for (int i = 0; i < NUM_ZOMBIES; i++) {
        launch_zombie(i);
    }
    ssigaction(SIGINT, handleSIGINT);
    printf("Appuyer sur CTRL+D pour terminer le programme...\n");
    while (getchar() != EOF);
    terminate_zombies();
    return 0;
}