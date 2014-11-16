#ifndef 	CHAT_H_
#define 	CHAT_H_

#include "common.h"

void Chat(int socket){
  int n, maxfd = -1;
  fd_set rfds;
  char message[MESSAGE_MAX_SIZE];
  while(1){
      /*** initiaize FD ****/
      FD_ZERO(&rfds);
      FD_SET(0, &rfds);
      maxfd = 0;
      FD_SET(socket, &rfds);
      if (socket > maxfd){
        maxfd = socket;
      }
      n = select(maxfd + 1, &rfds, NULL, NULL, NULL);
      if(n == -1){
        printf("ERROR: client select failed\n");
        break;
      }else if(n == 0){
        continue;
      }else{
        // Check inputs of client
        if(FD_ISSET(0, &rfds)){
          bzero(message, MESSAGE_MAX_SIZE + 1);
          fgets(message, MESSAGE_MAX_SIZE, stdin);
          n = send(ConnectSocket, message, strlen(message)-1, 0);
          if(n<0){
            printf("ERROR: message was not sent to server\n");
          }
        }
        // Check received from server
        if(FD_ISSET(socket, &rfds)){
          bzero(message, MESSAGE_MAX_SIZE + 1);
          n = recv(socket, message, MESSAGE_MAX_SIZE, 0);
          if(n > 0){
            printf("%s\n",message);
          }else{
              if (n < 0)
                printf("ERROR: no message received from server\n");
            }
        }
      }
  }
}


#endif
