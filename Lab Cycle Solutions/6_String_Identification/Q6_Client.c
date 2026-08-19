#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(){
	char buffer[1024];
	struct sockaddr_in server_addr;
	socklen_t server_len = sizeof(server_addr);

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8746);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	sendto(sockfd, "Hello from Client...", 20, 0,
           (struct sockaddr *)&server_addr, server_len);

	memset(buffer, 0, sizeof(buffer));

	recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
             (struct sockaddr *)&server_addr, &server_len);

	printf("Message from server: %s\n", buffer);

	memset(buffer, 0, sizeof(buffer));

	printf("Enter new-gen sentence: ");
	scanf("%[^\n]s", buffer);

	printf("\nInputted sentence: %s\n", buffer);

	sendto(sockfd, buffer, strlen(buffer) + 1, 0,
           (struct sockaddr *)&server_addr, server_len);

	memset(buffer, 0, sizeof(buffer));

	recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
             (struct sockaddr *)&server_addr, &server_len);

	printf("Translated sentence from server: %s\n", buffer);

	close(sockfd);

	return 0;
}
