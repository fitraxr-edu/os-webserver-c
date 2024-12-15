#ifndef SERVER_H
#define SERVER_H

#define PORT 65432
#define BUFFER_SIZE 8192
#define PROCESS_POOL_SIZE 5

#include "HTTPRequest.h"
#include "Router.h"

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

void handle_client(int client_fd);

void process_worker(int server_fd);

#endif