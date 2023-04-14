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
