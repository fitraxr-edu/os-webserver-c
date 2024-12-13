#include "lib/Server.h"
#include "lib/HTTPRequest.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PORT 65432
#define BUFFER_SIZE 4096
#define PROCESS_POOL_SIZE 15

void handle_client(int client_fd)
{
    char request[BUFFER_SIZE];
    int bytes_read;

    bytes_read = recv(client_fd, request, sizeof(request) - 1, 0);
    if (bytes_read <= 0) {
        perror("Failed to read from client");
        close(client_fd);
        return;
    }

    request[bytes_read] = '\0';  // Null-terminate string
    printf("Permintaan dari client:\n%s\n", request);

    // Mengirim Header HTTPResponse
    char header[BUFFER_SIZE];
    snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n\r\n"
    );

    HTTPRequest httprequest;
    httprequest = parse_http_request(request);
    printf("==REQUEST==\n"
        "Client: %d\n"
        "Method: %s\n"
        "URI: %s\n"
        "Version: %s\n"
        "Header: %s\n"
        "Body: %s\n",
        client_fd,
        httprequest.method,
        httprequest.URI,
        httprequest.HTTPVersion,
        httprequest.header,
        httprequest.body
    );

    send(client_fd, header, strlen(header), 0);

    close(client_fd);
}

void process_worker(int server_fd) {
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
    }

    handle_client(client_fd);
    close(client_fd);
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