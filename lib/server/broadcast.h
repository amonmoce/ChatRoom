#ifndef 	BROADCAST_H_
#define 	BROADCAST_H_

//broadcast
void broadcast(ConnectedUser* usersList, char broadcast_message[MESSAGE_MAX_SIZE]){
  int i;
  for(i=0;i<MAX_ONLINE_USER;i++){
    if(usersList[i].isOnline){
        send(usersList[i].socket, broadcast_message, strlen(broadcast_message), 0);
    }
  }
}

// broadcast except me
void broadcast_except(ConnectedUser* usersList, char broadcast_message[MESSAGE_MAX_SIZE], int socket){
  int i;
  for(i=0;i<MAX_ONLINE_USER;i++){
    if(usersList[i].isOnline && usersList[i].socket!= socket){
        send(usersList[i].socket, broadcast_message, strlen(broadcast_message), 0);
    }
  }
}

#endif
