#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

void fillLower(int n, int matrix[][n]);
void fillDiagonal(int n, int matrix[][n]);
void fillUpper(int n, int matrix[][n]);
void fillRandom(int n, int matrix[][n]);
void display(int n, int matrix[][n]);

void fillUpper(int n, int matrix[][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j >= i)
                matrix[i][j] = rand() % 50 + 1;
            else
                matrix[i][j] = 0;
        }
    }
}

void fillLower(int n, int matrix[][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j <= i)
                matrix[i][j] = rand() % 50 + 1;
            else
                matrix[i][j] = 0;
        }
    }
}

void fillDiagonal(int n, int matrix[][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                matrix[i][j] = rand() % 50 + 1;
            else
                matrix[i][j] = 0;
        }
    }
}

void fillRandom(int n, int matrix[][n]) {
    int choose = rand() % 3;

    if (choose == 0)
        fillUpper(n, matrix);
    else if (choose == 1)
        fillLower(n, matrix);
    else
        fillDiagonal(n, matrix);
}

void display(int n, int matrix[][n]) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

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

    srand(time(NULL));
    printf("Enter N = "); int n;
    scanf("%d", &n);

    int matrix[n][n];
    fillRandom(n, matrix);

    printf("\nMatrix Sent from Client: \n");
    display(n, matrix);

    send(sockFD, &n, sizeof(n), 0);
    send(sockFD, matrix, sizeof(matrix), 0);

    char output[1024];
    read(sockFD, output, sizeof(output));
    printf("\nMessage from Server: %s\n", output);

    close(sockFD);

    return 0;
}
