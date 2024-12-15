#ifndef ROUTER_H
#define ROUTER_H

#include "Server.h"

#define ROOT "/"
#define ROOT_PAGE "public/html/index.html"

#define REGISTER "/register"
#define REGISTER_PAGE "public/html/register.html"

#define PAGE_NOT_FOUND "public/html/404pagenotfound.html"

#define STYLE "/style/style.css"
#define STYLE_PAGE "public/style/style.css"

#define SUBMIT "/register"

void handle_404(int client_fd);

void handle_css(int client_fd);

void handle_root(int client_fd);
void handle_register(int client_fd);
void handle_submission(int client_fd, char *httprequestbody);

#endif