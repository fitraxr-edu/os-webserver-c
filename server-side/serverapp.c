#include "lib/Server.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 65432
#define BUFFER_SIZE 4096

int main()
{
    Server server;
    struct sockaddr_in client_addr;
    int client_fd;
    socklen_t client_addr_len = sizeof(client_addr);

    server = server_constructor(AF_INET, SOCK_STREAM, 0, PORT, 10);
    
    if (listen(server.server_fd, server.backlog) < 0) {
        perror("Listen failed");
        close(server.server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server running on localhost:%d\n", server.port);

    while (1) {
        client_fd = accept(server.server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client terhubung.\n");
    }

    close(client_fd);
    close(server.server_fd);

    return 0;
}