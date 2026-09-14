#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int port;

int main (void) {
	printf("Enter port: "); scanf("%d", &port);

	char buffer[BUFFER_SIZE]; struct sockaddr_in server_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);

	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;

	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

	if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Connection to server failed");
		return 1;
	}

	sendto(sock_fd, "Hello from client", 17, 0, (struct sockaddr*)NULL, sizeof(server_addr));

	memset(buffer, 0, sizeof(buffer));

	recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);

	printf("Message from server: %s\n", buffer);

	memset(buffer, 0, sizeof(buffer));

	close(sock_fd);
	return 0;
}
