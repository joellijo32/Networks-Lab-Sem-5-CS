#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


}

int main(){
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0){
		printf("Socket Creation Failed !\n");
		return 1;
	}
	struct sockaddr_in address;
	socklen_t len = sizeof(address);

	char buffer[1024];
	address.sin_family = AF_INET;
	address.sin_port = htons(8085);
	address.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock_fd, (struct addrinfo*)&address, sizeof(address)) < 0){
		printf("Bind Failed !\n");
		close(sock_fd);
		return 1;
	}

	listen(sock_fd, 5);

	printf("\nWaiting for Client....\n");

	int newsock = accept(sock_fd, (struct addrinfo*)&address, &len);

	if(newsock < 0){
		printf("Accept Failed !\n");
		close(sock_fd);
		return 1;
	}

	memset(buffer, 0, sizeof(buffer));

	read(newsock, buffer, sizeof(buffer));

	printf("Message from client: %s\n", buffer);

	send(newsock, "Hello from server", 17, 0);

	memset(buffer, 0, sizeof(buffer));

	read(newsock, buffer, sizeof(buffer));

	translate(buffer);

	send(newsock, buffer, strlen(buffer), 0);

	close(sock_fd);
	return 0;
}
