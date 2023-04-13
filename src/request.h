//
// Created by Ben Krocké on 01/04/2023.
//

#ifndef WEB_SERVER_REQUEST_H
#define WEB_SERVER_REQUEST_H

enum http_method {
    GET,
    POST,
    OPTIONS,
    HEAD
};

static const char *HTTP_METHOD_GET = "GET";
static const char *HTTP_METHOD_POST = "POST";
static const char *HTTP_METHOD_OPTIONS = "OPTIONS";
static const char *HTTP_METHOD_HEAD = "HEAD";


struct http_request {
    enum http_method method;
    char *path;
    char version[9];
    struct http_header *headers;
    char *body;
};

struct http_request parse_request(char *request);

#endif //WEB_SERVER_REQUEST_H
