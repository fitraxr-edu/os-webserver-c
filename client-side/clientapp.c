#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "lib/client.h"

#define PORT 65432
#define BUFFER_SIZE 4096

int main() 
{
    Client client = client_constructor(AF_INET, SOCK_STREAM, 0, PORT);

    char request[BUFFER_SIZE], response[BUFFER_SIZE];

    snprintf(request, sizeof(request),
             "GET / HTTP/1.1\r\n"
             "Host: localhost\r\n"
             "Connection: close\r\n\r\n");

    send(client.client_fd, request, strlen(request), 0);
    printf("Permintaan dikirim ke server:\n%s", request);

    int n = recv(client.client_fd, response, sizeof(response), 0);
    if (n > 0) {
        response[n] = '\0'; // Null-terminate string
        printf("Response: %s", response);
    }

    close(client.client_fd);

    return 0;
}