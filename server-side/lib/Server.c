#include "Server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void handle_client(int client_fd)
{
    char request[BUFFER_SIZE];
    int bytes_read;

    memset(request, 0, BUFFER_SIZE);
    bytes_read = recv(client_fd, request, sizeof(request) - 1, 0);
    if (bytes_read <= 0) {
        perror("Failed to read from client");
        close(client_fd);
        return;
    }

    if (bytes_read >= BUFFER_SIZE) {
        fprintf(stderr, "Request too large for buffer\n");
        close(client_fd);
        return;
    }

    request[bytes_read] = '\0';  // Null-terminate string

    HTTPRequest httprequest;
    httprequest = parse_http_request(request);
    printf("==REQUEST==\n"
        "Client: %d\n"
        "Method: %s\n"
        "URI: %s\n"
        "Version: %s\n"
        "Header: \n%s\n"
        "Body: %s\n\n"    ,
        client_fd,
        httprequest.method,
        httprequest.URI,
        httprequest.HTTPVersion,
        httprequest.header,
        httprequest.body
    );

    if (strcmp(httprequest.method, "GET") == 0)
    {
        if (strcmp(httprequest.URI, ROOT) == 0)
            handle_root(client_fd);
        else if (strcmp(httprequest.URI, REGISTER) == 0)
            handle_register(client_fd);
        else if (strcmp(httprequest.URI, STYLE) == 0)
            handle_css(client_fd);
        // else if (strcmp(httprequest.URI, "/favicon.ico") == 0)
        //     handle_404(client_fd);
        else
            handle_404(client_fd);
    }
    else if (strcmp(httprequest.method, "POST") == 0)
    {
        if (strcmp(httprequest.URI, SUBMIT) == 0) 
        {
            handle_submission(client_fd, httprequest.body);
        }
        else
            handle_404(client_fd);
    }

    close(client_fd);
}

void process_worker(int server_fd)
{
    while (1)
    {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);

        int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
        if (client_fd < 0) {
            perror("Accept failed");
        }

        handle_client(client_fd);
    }    
}