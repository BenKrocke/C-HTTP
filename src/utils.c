//
// Created by Ben Krocké on 31/03/2023.
//

#include <libc.h>
#include "utils.h"

void terminate(char *error) {
    perror(error);
    exit(1);
}
