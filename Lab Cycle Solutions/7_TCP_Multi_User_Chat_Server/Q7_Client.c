#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


int main() {

    int sockFD = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFD < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(2095);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int status = connect(sockFD, (struct sockaddr *)&address, sizeof(address));

    if (status < 0) {
        perror("Connection failed");
        close(sockFD);
        return 1;
    }

    printf("Connection was successful...\n");

    char buffer[1024] = "Hello from client";

    send(sockFD, buffer, strlen(buffer), 0);

    memset(buffer, 0, sizeof(buffer));

    read(sockFD, buffer, sizeof(buffer));

    printf("Message from Server: %s\n", buffer);

    memset(buffer, 0, sizeof(buffer));

   /* ---------*/

    close(sockFD);

    return 0;
}
