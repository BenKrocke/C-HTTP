//
// Created by Ben Krocké on 01/04/2023.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "response.h"
#include "http_header.h"

struct response initialise_response() {
    struct response response;
    response.version = HTTP;
    response.headers = create_http_header("Access-Control-Allow-Origin", "*");
    return response;
}

void add_body(struct response *response, char *body) {
    response->body = body;
    if (body != NULL) {
        char content_length_value[256] = "";
        snprintf(content_length_value, sizeof content_length_value, "%zu", strlen(body));
        char *clvptr = malloc(strlen(content_length_value) + 1);
        strcpy(clvptr, content_length_value);
        response->headers = add_http_header(response->headers, "Content-Length", clvptr);
    }
}

char *response_to_string(struct response *response) {
    char rsp[4096] = ""; // TODO: Increase size
    strcat(strcat(strcat(strcat(rsp, response->version), " "), response->status), "\r\n");
    struct http_header *current_header = response->headers;
    while (current_header != NULL) {
        strcat(strcat(strcat(strcat(rsp, current_header->key), ": "), current_header->value), "\r\n");
        current_header = current_header->next;
    }
    strcat(rsp, "\r\n");
    if (response->body != NULL) {
        strcat(rsp, response->body);
    }

    char *str = malloc(strlen(rsp) + 1);
    strcpy(str, rsp);
    delete_http_headers(response->headers);
    return str;
}