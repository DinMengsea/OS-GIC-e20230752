
/* client.c — Simple TCP Client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9090
#define BUFFER_SIZE 1024

int main(void) {
    int sock_fd;
    struct sockaddr_in server_addr;

    char buffer[BUFFER_SIZE] = {0};
    const char *message = "Hello from client!";

    // 1. Create a TCP socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Client: socket created (fd=%d)\n", sock_fd);

    // 2. Configure the server address
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert the server IP address into binary format
    if (inet_pton(AF_INET, "127.0.0.1",
                  &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Connect to the server
    printf("Client: connecting to server on port %d...\n", PORT);

    if (connect(sock_fd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client: connected!\n");

    // 4. Send a message to the server
    ssize_t bytes_sent = send(sock_fd, message, strlen(message), 0);

    if (bytes_sent < 0) {
        perror("send");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client: sent message: \"%s\"\n", message);

    // 5. Receive the server's response
    ssize_t bytes_read = read(sock_fd, buffer, BUFFER_SIZE - 1);

    if (bytes_read < 0) {
        perror("read");
    } else if (bytes_read == 0) {
        printf("Client: server disconnected.\n");
    } else {
        buffer[bytes_read] = '\0';

        printf("Client: received from server: \"%s\"\n", buffer);
    }

    // 6. Close the socket
    close(sock_fd);

    printf("Client: connection closed.\n");

    return 0;
}
