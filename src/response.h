//
// Created by Ben Krocké on 01/04/2023.
//

#ifndef WEB_SERVER_RESPONSE_H
#define WEB_SERVER_RESPONSE_H

static char *HTTP = "HTTP/1.1";

static char *STATUS_OK = "200 OK";
static char *STATUS_BAD_REQUEST = "400 Bad Request";
static char *STATUS_NOT_FOUND = "404 Not Found";
static char *STATUS_ERROR = "500 Internal Server Error";

struct response;
struct response_builder;

struct response_builder *create_response_builder();

#endif //WEB_SERVER_RESPONSE_H
