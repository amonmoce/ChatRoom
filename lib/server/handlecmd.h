#ifndef 	HANDLECMD_H_
#define 	HANDLECMD_H_


// handle commands received from the client
void handlecmd(char received_message[MESSAGE_MAX_SIZE], int socket){
  char *cmd, *message, list[MESSAGE_MAX_SIZE], tmp[MESSAGE_MAX_SIZE];
  int m;
  strcpy(tmp, received_message);
  cmd = strtok(tmp, "\n");
  cmd = strtok(cmd, " ");
  if(strcmp(cmd, "who") == 0){
    bzero(message_to_send, MESSAGE_MAX_SIZE );
    for(m=0; m<MAX_ONLINE_USER; m++){
      if(usersList[m].isOnline){
        bzero(list, MESSAGE_MAX_SIZE );
        if(usersList[m].socket!=socket){
          sprintf(list, "%s%s%s%s%s%d%s",
            "[Server] ",usersList[m].name,
            " ", usersList[m].ip_address,
            "/", usersList[m].port, "\n");
        }
        else{
          sprintf(list, "%s%s%s%s%s%d%s",
            "[Server] ",usersList[m].name,
            " ", usersList[m].ip_address,
            "/", usersList[m].port, " -> me\n");
        }
        strcat(message_to_send, list);
      }
    }
    message_to_send[strlen(message_to_send)-1] = '\0';
    send(socket, message_to_send, strlen(message_to_send), 0);
  }
  else if(strcmp(cmd, "name") == 0){
    message = substring(received_message, 6, MESSAGE_MAX_SIZE-6);
    uid = findUser(usersList, socket);
    if(!name_validity(message) || strcmp(usersList[uid].name, message) == 0){
      sprintf(message_to_send, "%s%s%s%s",
        "[Server] ", usersList[uid].name, " is now known as ", message);
      broadcast_except(usersList, message_to_send, socket);
      strcpy(usersList[uid].name, message);
      sprintf(message_to_send, "%s%s",
        "[Server] You are now known as ", usersList[uid].name);
      send(socket, message_to_send, strlen(message_to_send), 0);
    }
    else if(name_validity(message) == 1){
      sprintf(message_to_send, "%s",
        "[Server] ERROR: Username cannot be anonymous.");
      send(socket, message_to_send, strlen(message_to_send), 0);
    }
    else if(name_validity(message) == 2){
      sprintf(message_to_send, "%s%s%s",
        "[Server] ERROR: ", message, " has been used by others.");
      send(socket, message_to_send, strlen(message_to_send), 0);
    }
    else if(name_validity(message) == 3 || name_validity(message) == 4){
      sprintf(message_to_send, "%s",
        "[Server] ERROR: Username can only consists of 2~12 English letters.");
      send(socket, message_to_send, strlen(message_to_send), 0);
    }
  }
  else if(strcmp(cmd, "tell") == 0){
    int uid_sender = findUser(usersList, socket);
    if(strcmp(usersList[uid_sender].name, "anonymous") == 0){
      sprintf(message_to_send, "%s",
        "[Server] ERROR: You are anonymous.");
      send(socket, message_to_send, strlen(message_to_send), 0);
    }
    else{
      message = substring(received_message, 6, MESSAGE_MAX_SIZE-6);
      char *receiver = strtok(message, " ");
      char username[MESSAGE_MAX_SIZE];
      strcpy(username, receiver);
      uid = findUserByName(usersList, receiver);
      if(strcmp(receiver, "anonymous") == 0){
        sprintf(message_to_send, "%s",
          "[Server] ERROR: The client to which you sent is anonymous.");
        send(socket, message_to_send, strlen(message_to_send), 0);
      }
      else if(uid == -1){
        sprintf(message_to_send, "%s",
          "[Server] ERROR: The receiver doesn't exist");
        send(socket, message_to_send, strlen(message_to_send), 0);
      }
      else{
        char *receiver_message = substring(received_message, 7+strlen(username), MESSAGE_MAX_SIZE-7-strlen(username));
        sprintf(message_to_send, "%s%s%s%s",
          "[Server] ", usersList[uid_sender].name, " tell you ", receiver_message);
        send(usersList[uid].socket, message_to_send, strlen(message_to_send), 0);
        sprintf(message_to_send, "%s",
          "[Server] SUCCESS: Your message has been sent.");
        send(socket, message_to_send, strlen(message_to_send), 0);
      }
    }

  }
  else if(strcmp(cmd, "yell") == 0){
    message = substring(received_message, 6, MESSAGE_MAX_SIZE-6);
    uid = findUser(usersList, socket);
    sprintf(message_to_send, "%s%s%s%s", "[Server] ",usersList[uid].name, " yell ", message);
    broadcast(usersList, message_to_send);
  }
  else{
    sprintf(message_to_send, "%s", "[Server] ERROR: Error command.");
    send(socket, message_to_send, strlen(message_to_send), 0);
  }
  memset(received_message, '\0' ,MESSAGE_MAX_SIZE);
}

#endif
