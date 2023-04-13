#include "src/server.h"

int main(void) {
    int socket_descriptor = server_launch();
    server_listen(socket_descriptor);

    return 0;
}
