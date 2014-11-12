#include "common.h"
#define NUMPORTS 10

#define LISTENQ 5

int main(){
	int i;
	struct sockaddr_in servaddr;
	//lists of sockets for incoming connections
	int serverSockets[NUMPORTS];
	//max_descriptor init
	int max_descriptor = -1;


int listenfd, client_fd, recv_len;
	char recv_buf[MAXLINE];



	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(55555);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) <0)
		printf("ERROR: Create socket error\n");

	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) <0)
		printf("ERROR: Bind error\n");

	listen(listenfd, LISTENQ);

	//for(i=0; i<NUMPORTS; i++){
		//create port socket
		//serverSockets[i] = SetUpTCPServerSocket();
	//}

	while(1){
		memset(recv_buf, 0, sizeof(recv_buf));

		if((client_fd = accept(listenfd, NULL, NULL)) <0){
			printf("ERROR: Accept client error\n");
			close(listenfd);
		}
		recv_len = read(client_fd, recv_buf, sizeof(recv_buf));

		if(recv_len <0){
			printf("ERROR: Read from client error\n");
			close(client_fd);
		}
		else if(recv_len == 0){
			printf("The client close the connection\n");
			close(client_fd);
		}
		else{
			//printf("Receive message from the client, message is %s\n", recv_buf);
			write(client_fd, recv_buf, strlen(recv_buf));
			close(client_fd);
		}

	}

	return 0;
}