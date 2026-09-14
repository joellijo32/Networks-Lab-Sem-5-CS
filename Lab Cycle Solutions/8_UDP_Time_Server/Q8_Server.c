#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 1024
int port;

int main (void) {
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);

	printf("Enter port: "); scanf("%d", &port);

	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock_fd < 0) {
		perror("Socket creation failed");
		return 1;
	}

	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;

	if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Server binding failed");
		return 1;
	}

	printf ("\nWaiting for client...\n");

	memset (buffer, 0, sizeof(buffer));

	recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);

	printf("\nMessage from client: %s\n", buffer);

	memset(buffer, 0, sizeof(buffer));

	sendto(sock_fd, "Hello from Server", 17, 0, (struct sockaddr*)&client_addr, client_len);

	memset(buffer, 0, sizeof(buffer));

	recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);

	printf("%s from client\n", buffer);

	close(sock_fd);
	return 0;
}
