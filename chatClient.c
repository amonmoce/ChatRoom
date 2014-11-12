#include "common.h"


int main(int argc, char **argv){
	struct sockaddr_in servaddr;
	struct hostent *host;
	int fd, recv_len;
	char recv_buf[MAXLINE], send_buf[MAXLINE];

	if(argc != 2)
		printf("Usage: ./client <IP addr>\n");

	memset(&servaddr, 0, sizeof(servaddr));

	host = gethostbyname(argv[1]);
	if(host == NULL)
		printf("ERROR: Get host by name error\n");

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(55555);
	memcpy(&servaddr.sin_addr, host->h_addr, host->h_length);


	while(strcmp(fgets(send_buf, sizeof(send_buf), stdin), "exit\n") != 0){
		if((fd = socket(AF_INET, SOCK_STREAM, 0)) <0)
			printf("ERROR: Create socket error\n");
		if(connect(fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) <0)
			printf("ERROR: Connect to the server error\n");
		
		memset(recv_buf, 0, sizeof(recv_buf));
		write(fd, send_buf, strlen(send_buf));
		recv_len = read(fd, recv_buf, sizeof(recv_buf));
		if(recv_len == 0)
			printf("Server close the connection\n");
		else if(recv_len <0)
			printf("ERROR: Read from the server error\n");
		else{
			printf("Receive message from the server, message is %s", recv_buf);
			//close(fd);
		}	
		memset(send_buf, 0, sizeof(send_buf));
	}


	return 0;
}