#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int client;
    char* client_name;
} clients;

clients client_sockets[MAX_CLIENTS];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(char *msg, int sender_fd) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i].client != 0 && client_sockets[i].client != sender_fd) {
            send(client_sockets[i].client, msg, strlen(msg), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    free(arg); // Free the memory allocated in main

    char buffer[BUFFER_SIZE];
    char name_buffer[64];

    // 1. First recv: get the username
    memset(name_buffer, 0, sizeof(name_buffer));
    int name_bytes = recv(client_fd, name_buffer, sizeof(name_buffer) - 1, 0);
    if (name_bytes <= 0) {
        close(client_fd);
        return NULL;
    }

    // 2. Store client_name safely in the array
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i].client == client_fd) {
            client_sockets[i].client_name = strdup(name_buffer);
            break;
        }
    }
    pthread_mutex_unlock(&lock);

    // Announce client joined
    char join_msg[BUFFER_SIZE];
    snprintf(join_msg, sizeof(join_msg), "[Server] %s has joined the chat.", name_buffer);
    printf("%s\n", join_msg);
    broadcast_message(join_msg, client_fd);

    // 3. Normal chat loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_received <= 0) {
            // Client disconnected: cleanup and notify
            char leave_msg[BUFFER_SIZE];
            snprintf(leave_msg, sizeof(leave_msg), "[Server] %s has left the chat.", name_buffer);
            printf("%s\n", leave_msg);

            pthread_mutex_lock(&lock);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i].client == client_fd) {
                    client_sockets[i].client = 0;
                    free(client_sockets[i].client_name);
                    client_sockets[i].client_name = NULL;
                    break;
                }
            }
            pthread_mutex_unlock(&lock);

            broadcast_message(leave_msg, client_fd);
            close(client_fd);
            break;
        }

        // Format message as "Username: message"
        char formatted_msg[BUFFER_SIZE + 80];
        snprintf(formatted_msg, sizeof(formatted_msg), "%s: %s", name_buffer, buffer);
        printf("%s\n", formatted_msg);
        broadcast_message(formatted_msg, client_fd);
    }

    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    int PORT;
    printf("Enter port: ");
    scanf(" %d", &PORT);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is listening...\n");

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&lock);
        int i;
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i].client == 0) {
                client_sockets[i].client = client_fd;
                client_sockets[i].client_name = NULL;
                break;
            }
        }
        pthread_mutex_unlock(&lock);

        if (i == MAX_CLIENTS) {
            printf("Max clients reached! Connection rejected.\n");
            close(client_fd);
        } else {
            // Allocate memory for thread argument to avoid race conditions
            int *new_sock = malloc(sizeof(int));
            *new_sock = client_fd;

            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, new_sock);
            pthread_detach(tid);
        }
    }

    close(server_fd);
    return 0;
}