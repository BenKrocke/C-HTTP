//
// Created by Ben Krocké on 01/04/2023.
//

#include <string.h>
#include "router.h"
#include "utils.h"

char *database = "Hello, World!";

char *router_process(struct http_request request) {
    char *response = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: 13\r\n\r\nHello, World!";
    if (request.method == GET) {
        strlcat(response, database, strlen(response)); // TODO: does not work
        return response;
    } else if (request.method == POST) {
        database = request.body;
        strlcat(response, database, strlen(response));
        return response;
    } else if (request.method == HEAD) {
        return response;
    } else if (request.method == OPTIONS) {
        return response;
    } else {
        terminate("router");
    }
}
