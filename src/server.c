//
// Created by Ben Krocké on 31/03/2023.
//

#include "server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <printf.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "utils.h"
#include "router.h"
#include "request.h"
#include "http_header.h"
#include "string_constants.h"

void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

struct addrinfo initialise_hints() {
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    return hints;
}

void initialise_signals() {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        terminate("sigaction");
    }
}

int server_launch() {
    int socket_descriptor, addrinfo_result;
    int yes = 1;
    struct addrinfo *res, *current;
    struct addrinfo hints;

    hints = initialise_hints();
    if ((addrinfo_result = getaddrinfo("127.0.0.1", "8000", &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo err: %socket_descriptor\n", gai_strerror(addrinfo_result));
        exit(1);
    }

    for (current = res; current != NULL; current = current->ai_next) {
        if ((socket_descriptor = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            terminate("setsockopt");
        }

        if (bind(socket_descriptor, res->ai_addr, res->ai_addrlen) == -1) {
            close(socket_descriptor);
            perror("bind");
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if (listen(socket_descriptor, 10) != 0) {
        terminate("listen");
    }

    initialise_signals();

    printf("server: waiting for connections...\n");
    return socket_descriptor;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void server_listen(int socket_descriptor) {
    struct sockaddr_storage their_addr;
    socklen_t their_addr_size = sizeof their_addr;
    char s[INET6_ADDRSTRLEN];

    int accepted_descriptor = accept(socket_descriptor, (struct sockaddr *) &their_addr,
                                     &their_addr_size);
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
    printf("server: got connection from %s\n", s);

    while (1) {
        char buffer[1024];
        size_t bytes = recv(accepted_descriptor, buffer, sizeof buffer - 1, 0);
        buffer[bytes] = NULL_TERMINATOR;
        struct http_request request = parse_request(buffer);

        char *response = router_process(request);


        ssize_t bytes_sent = send(accepted_descriptor, response, strlen(response), 0);
        if (bytes_sent == -1) {
            terminate("send");
        } else if (bytes_sent < strlen(response)) {
            terminate("send");
        };

        free(response);

        struct http_header *connection = find_header(request.headers, "Connection");
        if (connection != NULL && strcmp(connection->value, "close") == 0) {
            close(accepted_descriptor);
            delete_http_headers(request.headers);
            exit(1);
        } else {
            delete_http_headers(request.headers);
        }
    }
}

