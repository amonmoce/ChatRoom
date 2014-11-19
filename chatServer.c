#include "server_setup.h"
#include "chatServer.h"

int main(int argc, char **argv){
	SetUpServerToListenTo();

	FD_ZERO(&master);				            // empty master set
	FD_ZERO(&read_fds);			           // empty read_fds set
	FD_SET(0, &master);
	FD_SET(ServerSocket, &master);	 // add ServerSocket into master set
	fdmax=ServerSocket;							// set Server as maximum fd number

	for(i=0; i < MAX_ONLINE_USER; i++) usersList[i].isOnline=0;

	do{
		// Copy master to read_fds
		read_fds = master;

		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
			printf("ERROR: server select failed\n");
			exit(EXIT_FAILURE);
		}

		// Scan every existing connection
		for(i = 0; i <= fdmax; i++) {
			bzero(message_to_send, MESSAGE_MAX_SIZE );
			if (FD_ISSET(i, &read_fds)){
				if(i==ServerSocket){   // new connection
					if ((AcceptSocket = accept( ServerSocket, (struct sockaddr *)&client, &addr_len )) == -1)
						printf("ERROR: connection not accepted\n");
					else{
						// Broadcast a message
						broadcast(usersList, "Someone is coming");
						if((uid = addUser(usersList, AcceptSocket, inet_ntoa(client.sin_addr), client.sin_port))!=-1){
							sprintf(message_to_send,"%s%s%s%d",
								"[Server] Hello anonymous from ",
								inet_ntoa(client.sin_addr),
								"/", client.sin_port);
							send(AcceptSocket, message_to_send, strlen(message_to_send), 0);
							// Add user's socket into FD set
							FD_SET(AcceptSocket, &master);
							if (AcceptSocket > fdmax){
								fdmax = AcceptSocket;
							}
							NumOfConnectedUsers++;
						}
					}
				}
				else{ //existing connection
					bzero(message_received, MESSAGE_MAX_SIZE );
					n = recv(i, message_received, MESSAGE_MAX_SIZE, 0);
					// Cannot receive message from client
					if (n <= 0) {
						NumOfConnectedUsers--;
						if(n==0){    //the user is offline
							if((uid=findUser(usersList, i))!=-1){
								bzero(message_to_send, MESSAGE_MAX_SIZE );
								sprintf(message_to_send,"%s%s%s",
									"[Server] ",
									usersList[uid].name,
									" is offline.");
								// Erase the record
								usersList[uid].isOnline = 0;
								usersList[uid].socket = -1;
								memset(usersList[uid].name, '\0' ,MAX_NAME_SIZE);
								broadcast(usersList, message_to_send);
							}
						}
						else{		  //fail in receiving
							printf("ERROR: reception message from socket(%d) failed\n", i);
						}
						close(i);
						FD_CLR(i, &master);
					}
					// Can receive message from client
					else{
						if((uid=findUser(usersList, i))!=-1){
							//printf("%s\n", message_received);
							handlecmd(message_received, i);
						}


					}
				}

			}
		}

	}while(!server_on);

	close(ServerSocket);

	return 0;
}
