#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
	int socketFD = socket(AF_INET, SOCK_STREAM, 0);

	char* ip = "235.3.42.1";
	struct sockaddr_in address ;
	address.sin_family = AF_INET;
	address.sin_port = 2000;
	inet_pton(AF_INET, &address, &address.sin_addr.s_addr);

	int status = connect(socketFD, &address, sizeof(address));

	if(status == 0) printf("\nConnection was successfull...\n");

	return 0;
}

