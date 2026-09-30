#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 256

int port;

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    char filename[BUFFER_SIZE];
    pid_t pid = getpid();

    int n = read(client_sock, filename, sizeof(filename) - 1);
    if (n <= 0) {
        close(client_sock);
        exit(0);
    }
    filename[n] = '\0';
    filename[strcspn(filename, "\r\n")] = 0; 

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        snprintf(buffer, sizeof(buffer), "[Server PID: %d] Error: File '%.100s' not found.\n", pid, filename);
        write(client_sock, buffer, strlen(buffer));
    } else {
        snprintf(buffer, sizeof(buffer), "[Server PID: %d] File Contents:\n----------------------------------------\n", pid);
        write(client_sock, buffer, strlen(buffer));

        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), f)) > 0) {
            write(client_sock, buffer, bytes_read);
        }
        fclose(f);
    }

    close(client_sock);
    exit(0); 
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    printf("Enter port: "); scanf("%d", &port);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Concurrent File Server started on port %d...\n", port);

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        if (fork() == 0) {
            close(server_sock); 
            handle_client(client_sock);
        } else {
            close(client_sock); 
	}
    }

    close(server_sock);
    return 0;
}
