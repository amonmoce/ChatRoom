#ifndef 	CHATSERVER_H_
#define 	CHATSERVER_H_

#include <netinet/in.h>

// Substring function: it returns a pointer to the substring
char *substring(char *string, int position, int length) {
   char *pointer;
   int c;
   pointer = malloc(length+1);
   for (c = 0 ; c < position -1 ; c++)
      string++;
   for (c = 0 ; c < length ; c++){
      *(pointer+c) = *string;
      string++;
   }
   *(pointer+c) = '\0';
   return pointer;
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

// broadcast except me
void broadcast_except(ConnectedUser* usersList, char broadcast_message[MESSAGE_MAX_SIZE], int socket){
	int i;
	for(i=0;i<MAX_ONLINE_USER;i++){
		if(usersList[i].isOnline && usersList[i].socket!= socket){
				send(usersList[i].socket, broadcast_message, strlen(broadcast_message), 0);
		}
	}
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

//find user id by the name
int findUserByName(ConnectedUser* usersList, char *name){
	int i;
	for(i=0;i<MAX_ONLINE_USER;i++){
		if(strcmp(usersList[i].name, name)==0)
			return i;
	}
	return -1;
}

// check if the username exists already
int name_exist(char *string){
	int i;
	for(i=0;i<MAX_ONLINE_USER;i++){
		if(strcmp(usersList[i].name, string)==0)
			return 1;
	}
	return 0;
}

// check string length, has to be between 2 and 12
int length_check(char *string){
	char *p;
	int count=0;
	for(p=string; *p!='\0'; p++)
			count++;
	if(count < 2 || count > 12)
		return 0;
	else
		return 1;
}

// check if string characters are english alphabet
int is_english_alphabet(char *string){
	char *p;
	for(p=string; *p!='\0'; p++){
		if(!isalpha(*p))
			return 0;
	}
	return 1;
}

// check the validity of the string and return 0, 1, 2, 3, 4
int name_validity(char *string){
	if(strcmp(string, "anonymous") == 0)
		return 1;
	else if(name_exist(string))
		return 2;
	else if(!length_check(string))
		return 3;
	else if(!is_english_alphabet(string))
		return 4;
	return 0;
}

// add user and return user id if succeed or return -1 if failed
int addUser(ConnectedUser* usersList, int socket, char *ip_address, int port){
	int i;
	for(i=0;i<MAX_ONLINE_USER;i++){
		if(!usersList[i].isOnline){
			usersList[i].socket = socket;
			strcpy(usersList[i].name, "anonymous");
			strcpy(usersList[i].ip_address, ip_address);
			usersList[i].port = port;
			usersList[i].isOnline = 1;
			return i;
		}
	}
	return -1;
}

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
