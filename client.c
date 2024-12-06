#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PORT 65432
#define LOCALHOST "127.0.0.1"

typedef struct Client {
    char *clientname;
} Client;

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    // Buat socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket failed");
        return -1;
    }

    // Konfigurasi alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    // Connect ke server
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    char *message = "Hello from client!";
    send(sock_fd, message, strlen(message), 0);
    
    int n = read(sock_fd, buffer, 1024);
    printf("Message from server: %s\n", buffer);

    // Tutup socket
    close(sock_fd);
    return 0;
}
