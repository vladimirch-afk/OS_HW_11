#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 256

void handleError(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server IP> <Server Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        handleError("Failed to create socket");
    }

    // Configure server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[2]));
    
    if (inet_pton(AF_INET, argv[1], &serverAddress.sin_addr) <= 0) {
        handleError("Invalid server IP address");
    }

    printf("Connected to the server\n");

    while (1) {
        printf("Enter message: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            handleError("Failed to read input");
        }

        // Send message to server
        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
            handleError("Failed to send message");
        }

        if (strcmp(buffer, "The End\n") == 0) {
            printf("Connection terminated by client\n");
            break;
        }
    }

    close(clientSocket);
    return 0;
}
