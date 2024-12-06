#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/types.h>

#define PORT 65432
#define MAX_CLIENTS 10
#define PROCESS_POOL_SIZE 5

void handle_client(int, char*);
void process_worker(int, char*);

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024] = {0};

    // Buat socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return -1;
    }

    // Konfigurasi alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return -1;
    }

    printf("Waiting for a connection...\n");

    for (int i = 0; i < PROCESS_POOL_SIZE; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Proses anak menangani klien
            process_worker(server_fd, buffer);
            exit(0);
        }
    }

    while (1) {
        wait(NULL); // Menunggu proses anak yang berhenti
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

void handle_client(int client_fd, char *buffer) {
    int n = read(client_fd, buffer, 1024);
    printf("Message from client: %s\n", buffer);

    char *reply = "Hello from server!";
    send(client_fd, reply, strlen(reply), 0);
}

void process_worker(int server_socket, char *buffer) {
    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);

        // Menerima koneksi klien
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Tangani klien
        handle_client(client_socket, buffer);
    }
}