#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


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

    /* -------- */

    close(newsock);
    close(sockFD);

    return 0;
}