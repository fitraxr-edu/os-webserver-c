#include <stdio.h>
#include "lib/client.h"

#define PORT 65432

int main() 
{
    Client client = client_constructor(AF_INET, SOCK_STREAM, 0, PORT);

    // printf("GET / HTTP/1.1\r\n"
    //          "Host: localhost\r\n"
    //          "Connection: close\r\n\r\n");

    return 0;
}