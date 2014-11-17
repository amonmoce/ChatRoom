#ifndef 	CHATSERVER_H_
#define 	CHATSERVER_H_

#include "common.h"
#include <netinet/in.h>
#define MAX_ONLINE_USER 10
#define MAX_NAME_SIZE 40

int chat_app_port = DEFAULT_SERVER_PORT, ServerSocket, reuse_addr = 1;
struct sockaddr_in local;
typedef struct user{
	int socket;
	//int talkto;
	char name[MAX_NAME_SIZE];
	//int isWaitName;
	//int isWaitTalker;
	int isOnline;
}ConnectedUser;

// handle commands received from the client
void handlecmd(char received_message[MESSAGE_MAX_SIZE]){
	char *cmd;
	cmd = strtok(received_message, "\n");
	cmd = strtok(cmd, " ");
	if(strcmp(cmd, "who") == 0)
		printf("list all\n");
	else if(strcmp(cmd, "name") == 0)
		printf("change name\n");
	else if(strcmp(cmd, "tell") == 0)
		printf("private message\n");
	else if(strcmp(cmd, "yell") == 0)
		printf("broadcast message\n");
	else
		printf("[Server] ERROR: Error command.\n");
	memset(received_message, '\0' ,MESSAGE_MAX_SIZE);
}

// add user and return user id if succeed or return -1 if failed
int addUser(ConnectedUser* usersList, int socket){
	int i;
	for(i=0;i<MAX_ONLINE_USER;i++){
		if(!usersList[i].isOnline){
			usersList[i].socket = socket;
			strcpy(usersList[i].name, "anonymous");
			//usersList[i].talkto = -1;
			//usersList[i].isWaitName = 0;
			//usersList[i].isWaitTalker = 0;
			usersList[i].isOnline = 1;
			return i;
		}
	}
	return -1;
}
// find user id and return it or return -1 if failed
int findUser(ConnectedUser* usersList, int socket){
	int i;
	for(i=0;i<MAX_ONLINE_USER;i++){
		if(usersList[i].socket==socket)
			return i;
	}
	return -1;
}

//broadcast
void broadcast(ConnectedUser* usersList, char broadcast_message[MESSAGE_MAX_SIZE]){
	int i;
	for(i=0;i<MAX_ONLINE_USER;i++){
		if(usersList[i].isOnline){
				send(usersList[i].socket, broadcast_message, strlen(broadcast_message), 0);
		}
	}
}

void SetUpServerToListenTo(){
	// Socket creation
	ServerSocket=socket(AF_INET,SOCK_STREAM,0);
	if(ServerSocket==-1){
		printf("ERROR: server socket not created\n");
		exit(EXIT_FAILURE);
	}
	// Set re-bind Server immediately
	setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,sizeof(reuse_addr));
	// Host address and port
	local.sin_family=AF_INET;
	local.sin_addr.s_addr=INADDR_ANY;
	local.sin_port=htons(chat_app_port);
	// Bind Socket
	if(bind(ServerSocket,(struct sockaddr*)&local,sizeof(local)) == -1){
		printf("ERROR: bind failed\n");
		close(ServerSocket);
		exit(EXIT_FAILURE);
	}
	// Socket listener creation
	if(listen(ServerSocket,1)== -1){
		printf("ERROR: listen failed\n");
		close(ServerSocket);
		exit(EXIT_FAILURE);
	}
}

#endif
