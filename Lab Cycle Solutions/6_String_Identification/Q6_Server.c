#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

void translate(char* buffer){
	char result[1024] = {0}, word[100];
	int i = 0, j = 0;
	while(buffer[i] != '\0'){
		if(isalpha(buffer[i])){
			j = 0;
			while(isalpha(buffer[i])){
				word[j++] = buffer[i++];
			}
			word[j] = '\0';
			i--;

			if(strcmp(word, "tbh") == 0){
				strcpy(word, "to be honest");
			} else if(strcmp(word, "ig") == 0){
				strcpy(word, "I guess");
			} else if(strcmp(word, "tbf") == 0){
				strcpy(word, "to be fair");
			} else if(strcmp(word, "atm") == 0){
				strcpy(word, "at the moment");
			} else if(strcmp(word, "irl") == 0){
				strcpy(word, "in real life");
			} else if(strcmp(word, "lol") == 0){
				strcpy(word, "laugh out loud");
			} else if(strcmp(word, "asap") == 0){
				strcpy(word, "as soon as possible");
			} else if(strcmp(word, "omg") == 0){
				strcpy(word, "oh my God");
			} else if(strcmp(word, "ttyl") == 0){
				strcpy(word, "talk to you later");
			} else if(strcmp(word, "idk") == 0){
				strcpy(word, "I don't know");
			} else if(strcmp(word, "nvm") == 0){
				strcpy(word, "nevermind");
			}

			strcat(result, word);
		} else {
			result[strlen(result)] = buffer[i];
			result[strlen(result)+1] = '\0';
		}
		i++;
	}
	result[strlen(result)] = '\0';

	strcpy(buffer, result);
}

int main(){ 
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0){
		printf("Socket Creation Failed !\n");
		return 1;
	}
	struct sockaddr_in address, client_addr;
	socklen_t client_len = sizeof(client_addr);

	char buffer[1024];
	address.sin_family = AF_INET;
	address.sin_port = htons(8746);
	address.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
		printf("Bind Failed !\n");
		close(sock_fd);
		return 1;
	}

	printf("\nWaiting for Client....\n");

	memset(buffer, 0, sizeof(buffer));

	recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
                 (struct sockaddr *)&client_addr, &client_len);

	printf("Message from client: %s\n", buffer);

	sendto(sock_fd, "Hello from server", 17, 0,
               (struct sockaddr *)&client_addr, client_len);

	memset(buffer, 0, sizeof(buffer));

	recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
                 (struct sockaddr *)&client_addr, &client_len);

	translate(buffer);
	buffer[strlen(buffer)] = '\0';
	sendto(sock_fd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&client_addr, client_len);

	close(sock_fd);

}
