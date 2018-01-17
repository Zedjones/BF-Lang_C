cmake_minimum_required(VERSION 2.0)

project(brainfuck_c C)
set(brainfuck_c_VERSION_MAJOR 1)
set(brainfuck_c_VERSION_MINOR 1)
set(brainfuck_c_VERSION_PATCH 0)

option(MAKE_TESTS "Build the test executables.")

include_directories(headers/)
add_definitions("-Wall -Wextra -O2 -std=c99")
add_executable(brainfuck_c src/brainfuck_c.c src/cells.c src/brainfuck_utils.c src/alternatives.c)
install(TARGETS brainfuck_c RUNTIME DESTINATION bin)
install(FILES headers/brainfuck_utils.h headers/cells.h headers/alternatives.h DESTINATION "headers")

if(MAKE_TESTS)
	add_executable(cell_test tests/cell_test.c src/cells.c)
	install(TARGETS cell_test RUNTIME DESTINATION bin)
endif()
