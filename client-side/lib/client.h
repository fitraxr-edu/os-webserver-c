#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>

typedef struct Client
{
    int domain;
    int service;
    int protocol;
    int port;

    int client_fd;
    struct sockaddr_in server_addr;
} Client;

Client client_constructor(int domain, int service, int protocol, int port);

#endif