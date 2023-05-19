#ifndef _NETWORK_H_
#define _NETWORK_H_

/// Module à compiler avec les feature test macros suivantes, selon la machine:
/// (cf. man 7 feature_test_macros)
///
///    -D_DEFAULT_SOURCE
///
/// OU si _DEFAULT-SOURCE n'est pas définie:
///
///    -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809

#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/ipc.h>
#include <poll.h>


// PRE : serverIP   : the server IP address 
//       serverPort : the port for the socket
//       sockfd   : socket file descriptor sockfd successufully created with ssocket.
// POST : on success connect a socket to the specified address and port 
//        on failure, displays the error cause on stderr and exit
int ssconnect(char *serverIP,int serverPort, int sockfd );

#endif  // _NETWORK_H_