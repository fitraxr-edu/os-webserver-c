#ifndef HTTPRequest_H
#define HTTPRequest_H

#define REQUEST_LINE_LEN 32
#define HEADER_LEN 256
#define BODY_LEN 256

#define METHOD_LEN 8
#define URI_LEN 128
#define HTTPVERSION_LEN 4

enum HTTPMethods 
{
    GET,
    POST
};

typedef struct HTTPRequest
{
    char request_line[REQUEST_LINE_LEN];
    char header[HEADER_LEN];
    char body[BODY_LEN];

    // Request line attributes
    char method[METHOD_LEN];
    char URI[URI_LEN];
    char HTTPVersion[HTTPVERSION_LEN];
} HTTPRequest;

void parse_request(const char *request, char *request_line, char *header, char *body);
void parse_request_line(char *request_line, char *method, char *URI, char *version);

HTTPRequest parse_http_request(char *request);

#endif