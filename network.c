#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>

#include "network.h"


int ssconnect(char *serverIP,int serverPort, int sockfd ){
  struct sockaddr_in addr;
  memset(&addr,0,sizeof(addr)); /* en System V */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(serverPort);
  inet_aton(serverIP,&addr.sin_addr);
  int ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
  //checkNeg(ret,"connect client error");
  return ret;
}
