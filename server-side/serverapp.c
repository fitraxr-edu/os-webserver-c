#include "lib/Server.h"
#include "lib/HTTPRequest.h"
#include "lib/Router.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

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