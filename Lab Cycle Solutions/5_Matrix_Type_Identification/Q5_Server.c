#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int checkMatrix(int n, int matrix[][n]){
	int isUpper = 1, isLower = 1, isDiagonal = 1;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n;j++){
			if(i < j && matrix[i][j] != 0) isLower = 0;
			if(i > j && matrix[i][j] != 0) isUpper = 0;
			if(i != j && matrix[i][j] != 0) isDiagonal = 0;
		}
	}
	if(isDiagonal) return 2;
	else if(isUpper) return 0;
	else if(isLower) return 1;
}

int main() {
    int sockFD = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFD < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in address, client;
    socklen_t len = sizeof(client);

    char buffer[1024];
    address.sin_family = AF_INET;
    address.sin_port = htons(2095);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockFD, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(sockFD);
        return 1;
    }

    listen(sockFD, 5);

    printf("Waiting for client...\n");

    int newsock = accept(sockFD, (struct sockaddr *)&client, &len);

    if (newsock < 0) {
        perror("Accept failed");
        close(sockFD);
        return 1;
    }

    memset(buffer, 0, sizeof(buffer));

    read(newsock, buffer, sizeof(buffer));

    printf("Message: %s\n", buffer);

    send(newsock, "Hello from server", 17, 0);

    int n;
    read(newsock, &n, sizeof(n));
    int matrix[n][n];
    read(newsock, &matrix, sizeof(matrix));

    int result = checkMatrix(n, matrix); char* output = "";

    printf("The matrix recieved is: ");
    if(result == 0) output = "Upper Traingle";
    else if(result == 1) output = "Lower Triangle";
    else output = "Diagonal";

    send(newsock, output, strlen(output), 0);

    close(newsock);
    close(sockFD);

    return 0;
}
