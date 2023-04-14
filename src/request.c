//
// Created by Ben Krocké on 01/04/2023.
//

#include <string.h>
#include <printf.h>
#include "request.h"
#include "utils.h"
#include "http_header.h"
#include "string_constants.h"

char *sep_by_str(char **string_ptr, char *delim) {
    if (*string_ptr == NULL) {
        return NULL;
    }

    char *new_ptr, *original_ptr;

    original_ptr = *string_ptr;

    new_ptr = strstr(*string_ptr, delim);

    if (new_ptr == NULL) {
        *string_ptr = NULL;
    } else {
        *new_ptr = NULL_TERMINATOR;
        new_ptr = new_ptr + strlen(delim);

        *string_ptr = new_ptr;
    }

    return original_ptr;
}

enum http_method parse_method(char *method) {
    if (strcmp(method, HTTP_METHOD_GET) == 0) {
        return GET;
    } else if (strcmp(method, HTTP_METHOD_POST) == 0) {
        return POST;
    } else if (strcmp(method, HTTP_METHOD_HEAD) == 0) {
        return HEAD;
    } else if (strcmp(method, HTTP_METHOD_OPTIONS) == 0) {
        return OPTIONS;
    } else {
        terminate("Parse HTTP");
    }
}

struct http_header *parse_headers(char *headers) {
    char *header;
    struct http_header *first_header = NULL;
    while ((header = sep_by_str(&headers, CRLF)) != NULL) {
        char *key = sep_by_str(&header, COLON_SPACE);
        if (first_header == NULL) {
            first_header = create_http_header(key, header);
        } else {
            first_header = add_http_header(first_header, key, header);
        }
    }

    return first_header;
}

struct http_request parse_request(char *request) {
    struct http_request result;
    char *status_line, *headers, *body = NULL;
    status_line = sep_by_str(&request, CRLF);
    headers = sep_by_str(&request, CRLFCRLF);
    if (*request != NULL_TERMINATOR) {
        body = request;
    }

    result.method = parse_method(strsep(&status_line, SPACE));
    result.path = strsep(&status_line, SPACE);
    strncpy(result.version, status_line, 9);

    result.headers = parse_headers(headers);
    result.body = body;

    return result;
}
