
/* server.c — Simple TCP Server */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9090
#define BUFFER_SIZE 1024

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in address;
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);

    char buffer[BUFFER_SIZE] = {0};
    const char *response = "Hello from server!";

    // 1. Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Server: socket created (fd=%d)\n", server_fd);

    // 2. Allow port reuse
    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Configure server address
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 4. Bind socket to port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: bound to port %d\n", PORT);

    // 5. Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: listening... waiting for a client.\n");

    // 6. Accept a client connection
    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_address,
                       &addrlen);

    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: client connected!\n");

    // 7. Receive message from client
    int bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);

    if (bytes_read < 0) {
        perror("read");
    } else if (bytes_read == 0) {
        printf("Server: client disconnected.\n");
    } else {
        buffer[bytes_read] = '\0';

        printf("Server: received from client: \"%s\"\n", buffer);

        // 8. Send response to client
        ssize_t bytes_sent = send(client_fd, response,
                                  strlen(response), 0);

        if (bytes_sent < 0) {
            perror("send");
        } else {
            printf("Server: sent response: \"%s\"\n", response);
        }
    }

    // 9. Close sockets
    close(client_fd);
    close(server_fd);

    printf("Server: connection closed.\n");

    return 0;
}
