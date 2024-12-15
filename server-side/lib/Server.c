#include "Server.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

Server server_constructor(int domain, int service, int protocol, int port, int backlog)
{
    Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.port = port;
    server.backlog = backlog;

    server.server_fd = socket(server.domain, server.service, server.protocol);
    if (server.server_fd < 0)
    {
        perror("Socket Creation Failed.");
        exit(1);
    }

    server.server_addr.sin_family = server.domain;
    server.server_addr.sin_port = htons(server.port);
    server.server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server.server_fd, (struct sockaddr *)&server.server_addr, sizeof(server.server_addr)) < 0) {
        perror("Bind failed");
        close(server.server_fd);
        exit(1);
    }

    return server;
}