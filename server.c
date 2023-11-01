#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-domain") != 0) {
        fprintf(stderr, "Usage: %s -domain <socket_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *socket_path = argv[2];

    int server_fd, client_fd;
    socklen_t client_len;
    struct sockaddr_un server_addr, client_addr;
    char buffer[256];

    // Create the socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Server: socket");
        exit(EXIT_FAILURE);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path, sizeof(server_addr.sun_path) - 1);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Server: bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("Server: listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for connections...\n");

    // Accept client connections
    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("Server: accept");
        exit(EXIT_FAILURE);
    }

    // Read data from the client
    ssize_t num_bytes = read(client_fd, buffer, sizeof(buffer));
    if (num_bytes == -1) {
        perror("Server: read");
        exit(EXIT_FAILURE);
    }

    // Null-terminate the received data
    buffer[num_bytes] = '\0';

    // Remove non-printable characters
//    removeNonPrintableChars(buffer);

    printf("Received from client: %s\n", buffer);

    // Execute the received command
    printf("Executing command: %s\n", buffer);
    system(buffer);

    // Clean up
    close(client_fd);
    close(server_fd);
    unlink(socket_path);

    return 0;
}