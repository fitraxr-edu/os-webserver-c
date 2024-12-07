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
#define PROCESS_POOL_SIZE 10
#define BUFFER_SIZE 1024

#define INDEX_HTML "html/index.html"

void send_response(int client_fd, const char *status, const char *content_type, const char *body);
void handle_client(int client_fd);
void process_worker(int server_fd);

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
            process_worker(server_fd);
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

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    int n = read(client_fd, buffer, 1024);
    printf("Request:\n" 
            "%s", buffer);

    char *reply = "Hello from server!";
    send_response(client_fd, "OK", "HTML", "<html></html>");
    // send(client_fd, reply, strlen(reply), 0);
}

void process_worker(int server_fd) {
    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);

        // Menerima koneksi klien
        int client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Tangani klien
        handle_client(client_socket);
    }
}

void send_response(int client_fd, const char *status, const char *content_type, const char *body) {
    FILE *fp = fopen(INDEX_HTML, "r");
    if (!fp)
    {
        perror("Failed to open file");
        const char *error_response = "HTTP/1.1 404 Not Found\r\n\r\nFile not found";
        send(client_fd, error_response, strlen(error_response), 0);
        close(client_fd);
        return;
    }
    
    const char *header = "HTTP/1.1 %s\r\n"
                         "Content-Type: %s\r\n"
                         "Content-Length: %d\r\n"
                         "Connection: close\r\n\r\n"
                         "%s\n";
    send(client_fd, header, strlen(header), 0);

    // Membuat header HTTP
    char file_buffer[BUFFER_SIZE];
    while (fgets(file_buffer, sizeof(file_buffer), fp)) {
        send(client_fd, file_buffer, strlen(file_buffer), 0);
    }
    
    fclose(fp);
    close(client_fd);
    printf("Respons dikirim ke client.\n");
}