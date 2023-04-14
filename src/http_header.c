//
// Created by Ben Krocké on 13/04/2023.
//

#include <string.h>
#include <printf.h>
#include "http_header.h"

void delete_http_headers(struct http_header *headers) {
    if (headers->next != NULL) {
        delete_http_headers(headers->next);
    }
    free(headers->key);
    free(headers->value);
    free(headers);
}

struct http_header *add_http_header(struct http_header *headers, char *key, char *value) {
    struct http_header *header = create_http_header(key, value);
    if (header != NULL) {
        header->next = headers;
    }
    return header;
}

struct http_header *create_http_header(char *key, char *value) {
    struct http_header *headers = malloc(sizeof(struct http_header));
    if (headers != NULL) {
        headers->key = malloc(strlen(key) + 1);
        headers->value = malloc(strlen(value) + 1);
        strcpy(headers->key, key);
        strcpy(headers->value, value);
        headers->next = NULL;
    }
    return headers;
}

struct http_header *find_header(struct http_header *headers, char *key) {
    if (strcmp(headers->key, key) == 0) {
        return headers;
    }
    if (headers->next != NULL) {
        return find_header(headers->next, key);
    }
    return NULL;
}

