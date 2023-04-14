cmake_minimum_required(VERSION 3.24)
project(web_server C)

set(CMAKE_C_STANDARD 17)

add_executable(web_server main.c src/utils.c src/utils.h src/server.c src/server.h src/router.c src/router.h src/response.h src/response.c src/request.c src/request.h src/http_header.c src/http_header.h src/string_constants.h)
