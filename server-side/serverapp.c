#include "lib/Server.h"
#include "lib/HTTPRequest.h"
#include "lib/Router.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

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

int main()
{

    Server server;

    server = server_constructor(AF_INET, SOCK_STREAM, 0, PORT, 10);
    
    if (listen(server.server_fd, server.backlog) < 0) {
        perror("Listen failed");
        close(server.server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server running on localhost:%d\n", server.port);

    for (int i = 0; i < PROCESS_POOL_SIZE; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            process_worker(server.server_fd);
            exit(0);
        }
    }

    while (1) {
        wait(NULL); // Menunggu proses anak yang berhenti
    }

    close(server.server_fd);

    return 0;
}