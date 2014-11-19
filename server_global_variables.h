#ifndef 	SERVER_GLOBAL_VARIABLES_H_
#define 	SERVER_GLOBAL_VARIABLES_H_

#include "common.h"
#define MAX_ONLINE_USER 10
#define MAX_NAME_SIZE 40
#define MAX_IP_LENGTH 20

int chat_app_port = DEFAULT_SERVER_PORT, ServerSocket, reuse_addr = 1;
struct sockaddr_in local;
int i, server_on = 0, NumOfConnectedUsers = 0, n;
int fdmax;					   // maximum fd number
int AcceptSocket; 		// client socket
int uid;						 // user id
// send/receive buffer
char message_received[MESSAGE_MAX_SIZE], message_to_send[MESSAGE_MAX_SIZE];
// For getting user information
struct sockaddr_in client;
socklen_t addr_len = sizeof(client);
// FD sets
fd_set read_fds;				               // update by master
fd_set master;					              // update by new connection or disconnection

typedef struct user{
  int socket;
  int port;
  char name[MAX_NAME_SIZE];
  char ip_address[MAX_IP_LENGTH];
  int isOnline;
}ConnectedUser;

// Allocate and initialize array of users
ConnectedUser usersList[MAX_ONLINE_USER];


#endif
