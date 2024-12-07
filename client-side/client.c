#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 65432
#define LOCALHOST "127.0.0.1"
#define BUFFER_SIZE 4096

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    char request[BUFFER_SIZE], response[BUFFER_SIZE];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    printf("Connected to server on %s:%d\n", LOCALHOST, PORT);

    snprintf(request, sizeof(request),
             "GET / HTTP/1.1\r\n"
             "Host: localhost\r\n"
             "Connection: close\r\n\r\n");

    if (send(client_fd, request, strlen(request), 0) < 0)
    {
        printf("Gagal mengirim request.");
        return -1;
    }
    printf("Permintaan dikirim ke server:\n%s", request);

    if (read(client_fd, buffer, BUFFER_SIZE) < 0)
    {
        printf("Gagal menerima response.\n");
        return -1;
    }

    ssize_t bytes_received;
    while ((bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';  // Tambahkan null-terminator
    }

    printf("Response dari server:\n"
            "%s\n", buffer);

    close(client_fd);
    return 0;
}