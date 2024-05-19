#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define SERVER_IP "127.0.0.1"

void handleError(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int serverSocket;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[BUFFER_SIZE];
    socklen_t clientAddressLength = sizeof(clientAddress);

    // Create UDP socket
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        handleError("Failed to create socket");
    }

    // Configure server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddress.sin_port = htons(atoi(argv[1]));

    // Bind socket to address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        handleError("Failed to bind socket");
    }

    printf("Server running on IP: %s, Port: %d\n", SERVER_IP, ntohs(serverAddress.sin_port));

    while (1) {
        // Receive message from client
        int messageLength = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (messageLength < 0) {
            handleError("Failed to receive message");
        }
        buffer[messageLength] = '\0';
        printf("Received message: %s\n", buffer);

        if (strcmp(buffer, "The End\n") == 0) {
            printf("Connection terminated by client\n");
            break;
        }
    }

    close(serverSocket);
    return 0;
}
