//
// Created by Ben Krocké on 01/04/2023.
//

#ifndef WEB_SERVER_RESPONSE_H
#define WEB_SERVER_RESPONSE_H

#include "request.h"

static char *HTTP = "HTTP/1.1";

static char *STATUS_OK = "200 OK";
static char *STATUS_NO_CONTENT = "204 No Content";
static char *STATUS_CONTENT_TOO_LARGE = "413 Content Too Large";
static char *STATUS_BAD_REQUEST = "400 Bad Request";
static char *STATUS_NOT_FOUND = "404 Not Found";
static char *STATUS_ERROR = "500 Internal Server Error";

struct response {
    char *version;
    char *status;
    struct http_header *headers;
    char *body;
};

struct response initialise_response();

void add_body(struct response *response, char *body);

char *response_to_string(struct response *response);

#endif //WEB_SERVER_RESPONSE_H
