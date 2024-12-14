#include "Router.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper Function
FILE *open_file(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);

    if (!file)
    {
        perror("FILE NOT FOUND.");
        exit(1);
    }

    return file;
}

void serve_file(int client_fd, const char *filename, char *response)
{
    char buffer[BUFFER_SIZE];

    FILE *file = open_file(filename, "r");

    send(client_fd, response, strlen(response), 0);

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_fd, buffer, bytes_read, 0);
    }

    fclose(file);
}

// Route Handler

void handle_404(int client_fd)
{
    char response[] = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n";

    serve_file(client_fd, PAGE_NOT_FOUND, response);
}

void handle_css(int client_fd)
{
    char response[] = 
        "HTTP/1.1 200 OK"
        "Content-Type: text/css"
        "Connection: close";
    
    serve_file(client_fd, STYLE_PAGE, response);
}

void handle_root(int client_fd)
{
    char response[] = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n";

    serve_file(client_fd, ROOT_PAGE, response);
}

void handle_register(int client_fd)
{
    char response[] = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n";

    serve_file(client_fd, REGISTER_PAGE, response);
}

void handle_submission(int client_fd, char *httprequestbody)
{
    char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Data received and saved to CSV.";
    send(client_fd, response, strlen(response), 0);

    FILE *file = open_file("data/data_mahasiswa.csv", "a");
    char name[128], nim[9];

    sscanf(httprequestbody, "nim=%[^&]&name=%s", nim, name);

    fprintf(file, "%s,%s\n", nim, name);
}