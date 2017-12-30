/// file: brainfuck_c.c
/// description: run the brainfuck interpreter
/// @author Nicholas Jones dojoman19@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "cells.h"
#include "brainfuck_utils.h"

char* USAGE_MESSAGE = "brainfuck_c [-f brainfuck-file]";

int main(int argc, char* argv[]){
	int opt;
	char* program = NULL;
	FILE* programfile;
	if(argc != 1 && argc != 3){
		fprintf(stderr, "%s\n", USAGE_MESSAGE);
		return EXIT_FAILURE;
	}
	while((opt = getopt(argc, argv, "f:")) != -1){
		switch(opt){
			case 'f':
				if(program != NULL){
					fprintf(stderr, "%s\n", USAGE_MESSAGE);
					return EXIT_FAILURE;
				}
				program = optarg;
				break;
		}
	}
	LinkedList* list = create_cells();
	if(program != NULL){
		programfile = fopen(program, "r");
		if(programfile == NULL){
			perror(program);
			return EXIT_FAILURE;
		}
		process_input(programfile, list);	
		fclose(programfile);
	}
	else{
		process_input(stdin, list);	
	}
	destroy_list(list);
}
