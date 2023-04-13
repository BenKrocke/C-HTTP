//
// Created by Ben Krocké on 13/04/2023.
//

#ifndef WEB_SERVER_HTTP_HEADER_H
#define WEB_SERVER_HTTP_HEADER_H

#include <stdlib.h>

struct http_header {
    char *key;
    char *value;
    struct http_header *next;
};

struct http_header *create_http_header(char *key, char *value);

void delete_http_headers(struct http_header *headers);

struct http_header *add_http_header(struct http_header *headers, char *key, char *value);

struct http_header *find_header(struct http_header *headers, char *key);

#endif //WEB_SERVER_HTTP_HEADER_H
