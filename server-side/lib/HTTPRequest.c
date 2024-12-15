#include "HTTPRequest.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replace_plus_with_space(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '+') {
            str[i] = ' '; // Ganti '+' dengan spasi
        }
    }
}

void parse_query(const char *query, char *nim, char *name) {
    char buffer[1024]; // Buffer sementara untuk memproses data
    strcpy(buffer, query); // Salin query ke buffer agar tidak mengubah data asli

    char *token = strtok(buffer, "&"); // Pisahkan berdasarkan '&'

    while (token != NULL) {
        if (strncmp(token, "nim=", 4) == 0) {
            strncpy(nim, token + 4, 50); // Salin nilai nim
        } else if (strncmp(token, "name=", 5) == 0) {
            strncpy(name, token + 5, 100); // Salin nilai name
        }
        token = strtok(NULL, "&"); // Lanjut ke token berikutnya
    }

    replace_plus_with_space(name); // Ganti '+' dengan spasi di name
}

void parse_request(const char *request, char *request_line, char *header, char *body) 
{
    // Memisahkan request line (baris pertama)
    const char *line_end = strstr(request, "\r\n");
    if (!line_end) {
        fprintf(stderr, "Invalid HTTP request format: No request line.\n");
        exit(EXIT_FAILURE);
    }

    size_t request_line_len = line_end - request;
    strncpy(request_line, request, request_line_len);
    request_line[request_line_len] = '\0';

    // Mencari akhir header menggunakan "\r\n\r\n"
    const char *header_end = strstr(request + request_line_len + 2, "\r\n\r\n");
    if (!header_end) {
        fprintf(stderr, "Invalid HTTP request format: No header end.\n");
        exit(EXIT_FAILURE);
    }

    // Menyalin header ke buffer
    size_t header_len = header_end - (request + request_line_len + 2);
    strncpy(header, request + request_line_len + 2, header_len);
    header[header_len] = '\0';

    // Menyalin body
    const char *body_start = header_end + 4; // Body dimulai setelah "\r\n\r\n"
    strcpy(body, body_start);
}

void parse_request_line(char *request_line, char *method, char *URI, char *version)
{
    // Mengambil method request yang dikirm
    strcpy(method, strtok(request_line, " "));

    // Mengambil URI request yang dikirim
    strcpy(URI, strtok(NULL, " "));

    // Mengambil versi HTTP yang digunakan pada request
    strcpy(version, strtok(NULL, " "));
    strcpy(version, strtok(version, "/"));
    strcpy(version, strtok(NULL, "/"));
}

HTTPRequest parse_http_request(char *request)
{
    HTTPRequest httprequest;

    parse_request(request, httprequest.request_line, httprequest.header, httprequest.body);
    parse_request_line(request, httprequest.method, httprequest.URI, httprequest.HTTPVersion);

    return httprequest;
}