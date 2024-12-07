#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

typedef struct Server
{
    int domain;
    int service;
    int protocol;
    int port;
    int backlog;

    int server_fd;
    struct sockaddr_in server_addr;

} Server;

Server server_constructor(int domain, int service, int protocol, int port, int backlog);

#endif