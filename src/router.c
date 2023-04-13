//
// Created by Ben Krocké on 01/04/2023.
//

#include <string.h>
#include "router.h"
#include "utils.h"
#include "response.h"
#include "http_header.h"

char *database = "Hello, World!";

char *validate_request(struct http_request request) {
    struct http_header *content_length = find_header(request.headers, "Content-Length");
    if (content_length != NULL && atoi(content_length->value) > 1024 - 1) {
        struct response response = initialise_response();
        response.status = STATUS_CONTENT_TOO_LARGE;
        return response_to_string(&response);
    }

    return NULL;
}

char *router_process(struct http_request request) {

    struct response response = initialise_response();
    if (request.method == GET) {
        add_body(&response, database);
        if (response.body == NULL) {
            response.status = STATUS_NO_CONTENT;
        } else {
            response.status = STATUS_OK;
        }
        return response_to_string(&response);
    } else if (request.method == POST) {
        char *validation;
        if ((validation = validate_request(request)) != NULL) {
            return validation;
        }

        database = request.body;
        add_body(&response, database);
        if (response.body == NULL) {
            response.status = STATUS_NO_CONTENT;
        } else {
            response.status = STATUS_OK;
        }
        return response_to_string(&response);
    } else if (request.method == HEAD) {
        response.status = STATUS_OK;
        return response_to_string(&response);
    } else if (request.method == OPTIONS) {
        response.status = STATUS_OK;
        response.headers = add_http_header(response.headers, "Allow", "GET,POST,HEAD,OPTIONS");
        return response_to_string(&response);
    } else {
        terminate("router");
    }
}
