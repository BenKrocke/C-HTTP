//
// Created by Ben Krocké on 01/04/2023.
//

#include <string.h>
#include <printf.h>
#include <stdlib.h>
#include "request.h"
#include "utils.h"

// https://github.com/jflaherty/ptrtut13/blob/master/md/prefacex.md
// https://stackoverflow.com/questions/176409/build-a-simple-http-server-in-c
// TODO: Buffered reader?
// https://www.jmarshall.com/easy/http/

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
        *new_ptr = '\0';
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
    int length = 0;
    char *header;
    struct http_header *array = malloc(0);
    struct http_header *temp;
    while ((header = sep_by_str(&headers, "\r\n")) != NULL) {
        struct http_header parsed_header;
        char *key = sep_by_str(&header, ": ");
        parsed_header.key = key;
        parsed_header.value = header;
        length++;
        temp = realloc(array, length * sizeof(struct http_header));
        if (temp) {
            array = temp;
            array[length - 1] = parsed_header;
        } else {
            terminate("Memory problem");
        }
    }

    return array;
}

// TODO: Validation
struct http_request parse_request(char *request) {
    struct http_request result;
    char *status_line, *headers, *body = NULL;
    status_line = sep_by_str(&request, "\r\n");
    headers = sep_by_str(&request, "\r\n\r\n");
    if (*request != '\0') {
        body = request;
    }

    // Parse data
    result.method = parse_method(strsep(&status_line, " "));
    result.path = strsep(&status_line, " ");
    strncpy(result.version, status_line, 9);

    result.headers = parse_headers(headers);
    result.body = body;

    return result;
}
