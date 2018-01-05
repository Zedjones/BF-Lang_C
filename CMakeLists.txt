cmake_minimum_required(VERSION 2.0)

project(brainfuck_c C)
set(brainfuck_c_VERSION_MAJOR 1)
set(brainfuck_c_VERSION_MINOR 0)
set(brainfuck_c_VERSION_PATCH 0)

include_directories(/)
add_definitions("-Wall -Wextra -O2 -std=c99")
add_executable(brainfuck_c brainfuck_c.c cells.c brainfuck_utils.c)
install(TARGETS brainfuck_c RUNTIME DESTINATION bin)
