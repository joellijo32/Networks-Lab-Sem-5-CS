#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

int sock;
struct sockaddr_in addr;

void* handle_chat(void *varg) {
    char buff[BUFFER_SIZE];
    while (1) {
        memset(buff, 0, sizeof(buff));
        int valread = read(sock, buff, BUFFER_SIZE - 1);
        if (valread <= 0) {
            printf("\nDisconnected from server.\n");
            exit(0);
        }
        printf("%s\n", buff);
        printf("> ");
        fflush(stdout);
    }
    return NULL;
}

int main() {
    int PORT;
    printf("Enter PORT: ");
    scanf(" %d", &PORT);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket failed to open");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    if (connect(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("Connect failed");
        exit(1);
    }

    char username[64];
    printf("Enter your username: ");
    scanf(" %63s", username);
    send(sock, username, strlen(username), 0);

    pthread_t th_id;
    pthread_create(&th_id, NULL, handle_chat, NULL);

    while (1) {
        char msg[BUFFER_SIZE];
        printf("> ");
        scanf(" %[^\n]", msg);
        send(sock, msg, strlen(msg), 0);
    }

    pthread_join(th_id, NULL);
    close(sock);
    return 0;
}
