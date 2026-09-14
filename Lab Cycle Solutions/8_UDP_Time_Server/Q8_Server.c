#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>


#define BUFFER_SIZE 1024
int port;

int main (void) {
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr, client_addr;

	printf("Enter port: "); scanf("%d", &port);

	int sock_fd = socket (AF_INET, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if (sock_fd < 0) {
		perror("Socket creation failed");
		return 1;
	}

	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.port = htons(port);
	server_addr.sin_family = AF_INET;

	if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Server binding failed");
		return 1;
	}

	
