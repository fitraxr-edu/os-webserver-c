#ifndef HTTPRequest_H
#define HTTPRequest_H

enum HTTPMethods 
{
    GET,
    POST
};

typedef struct HTTPRequest
{
    // Request line attributes
    enum HTTPMethods method;
    char *URI;
    char *HTTPVersion;
} HTTPRequest;

void parse_request(const char *request, char *request_line, char *header, char *body);
void parse_request_line(char *request_line, char *method, char *URI, char *version);

#endif