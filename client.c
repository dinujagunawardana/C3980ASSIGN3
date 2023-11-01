#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char *argv[]) {
    if (argc != 4 || strcmp(argv[1], "-domain") != 0) {
        fprintf(stderr, "Usage: %s -domain <socket_path> <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *socket_path = argv[2];
    const char *command = argv[3];  // The command you want to send

    int client_fd;
    struct sockaddr_un server_addr;

    // Create the socket
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Client: socket");
        exit(EXIT_FAILURE);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path, sizeof(server_addr.sun_path) - 1);

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Client: connect");
        exit(EXIT_FAILURE);
    }

    // Send the command to the server
    if (write(client_fd, command, strlen(command)) == -1) {
        perror("Client: write");
        exit(EXIT_FAILURE);
    }

    printf("Sent command to server: %s\n", command);

    // Clean up
    close(client_fd);

    return 0;
}
