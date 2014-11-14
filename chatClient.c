#include "chatClient.h"

int main(int argc, char **argv){

	if(argc != 3){
		printf("Usage: ./chatClient <Server IP addr> <Server Port num>\n");
		exit(EXIT_FAILURE);
	}
	strcpy(server_ip, argv[1]);
	server_port = atoi(argv[2]);

	SetUpTCPConnection();

	return 0;
}
