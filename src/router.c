//
// Created by Ben Krocké on 01/04/2023.
//

#include <string.h>
#include "router.h"
#include "utils.h"
#include "response.h"
#include "http_header.h"

char *database = "Hello, World!";

char *router_process(struct http_request request) {
    struct response response = initialise_response();
    if (request.method == GET) {
        add_body(&response, database);
        response.status = STATUS_OK;
        return response_to_string(&response);
    } else if (request.method == POST) {
        database = request.body;
        add_body(&response, database);
        response.status = STATUS_OK;
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
