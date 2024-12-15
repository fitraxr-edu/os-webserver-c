#include "client.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LOCALHOST "127.0.0.1"
#define PORT 65432
#define BUFFER_SIZE 4096

Client client_constructor(int domain, int service, int protocol, int port)
{
    Client client;

    client.domain = domain;
    client.service = service;
    client.protocol = protocol;
    client.port = port;

    client.client_fd = socket(client.domain, client.service, client.protocol);
    if (client.client_fd < 0)
    {
        perror("Socket Creation Failed.");
        exit(1);
    }

    client.server_addr.sin_family = client.domain;
    client.server_addr.sin_port = htons(client.port);
    client.server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client.client_fd, (struct sockaddr *)&client.server_addr, sizeof(client.server_addr)) < 0)
    {
        perror("Connection failed");
        close(client.client_fd);
        exit(1);
    }

    return client;
}