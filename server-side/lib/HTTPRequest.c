#include "HTTPRequest.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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