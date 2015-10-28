#ifndef 	SERVER_SETUP_H_
#define 	SERVER_SETUP_H_

#include "server_global_variables.h"

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
