#ifndef 	CONNECTION_H_
#define 	CONNECTION_H_

#include "common.h"
#include <netdb.h>
#define MAX_SERVER_NAME_SIZE 20

int server_port, ConnectSocket, addr;
char server_ip[MAX_SERVER_NAME_SIZE];
struct hostent *host;
struct sockaddr_in server;

void SetUpTCPConnection(){
	//Socket creation
	ConnectSocket=socket(AF_INET,SOCK_STREAM,0);
	if(ConnectSocket==-1){
		printf("ERROR: client socket not created\n");
		exit(EXIT_FAILURE);
	}
	//Address conversion to machine format
	if(isalpha(server_ip[0])){ 					  //if the argument is string
		host= gethostbyname(server_ip);    //get ip address from host name
		if(host == NULL){
			printf("ERROR: unknown host\n");
			exit(EXIT_FAILURE);
		}
		addr=*host->h_addr;
	}
	else{
		addr=inet_addr(server_ip);
	}
	// Host address and port
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=addr;
	server.sin_port=htons(server_port);
	// Connect to Server
	if(connect(ConnectSocket,(struct sockaddr*)&server,sizeof(server))== -1){
		printf("ERROR: client not connected\n");
		exit(EXIT_FAILURE);
	}
}

#endif
