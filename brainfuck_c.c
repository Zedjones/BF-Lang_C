/// file: brainfuck_c.c
/// description: run the brainfuck interpreter
/// @author Nicholas Jones dojoman19@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "cells.h"
#include "brainfuck_utils.h"

//standard usage message and newline extern
char* USAGE_MESSAGE = "brainfuck_c [-f brainfuck-file] [-n]";
bool ZERO_NEWLINE;

/**
Process command line arguments, set up variables, and pass to 
process_input
*/
int main(int argc, char* argv[]){
	int opt;
	char* program = NULL;
	FILE* programfile;
	//return if incorrect number of arguments
	if(argc != 1 && argc != 3 && argc != 4){
		fprintf(stderr, "%s\n", USAGE_MESSAGE);
		return EXIT_FAILURE;
	}
	//process command line arguments
	while((opt = getopt(argc, argv, "nf:")) != -1){
		switch(opt){
			case 'f':
				if(program != NULL){
					fprintf(stderr, "%s\n", USAGE_MESSAGE);
					return EXIT_FAILURE;
				}
				program = optarg;
				break;
			case 'n':
				ZERO_NEWLINE = true;
				break;
		}
	}
	//create list of cells
	LinkedList* list = create_cells();
	//if there's a program to read
	if(program != NULL){
		//open the program and pass to process_input
		//with the file
		programfile = fopen(program, "r");
		if(programfile == NULL){
			perror(program);
			return EXIT_FAILURE;
		}
		process_input(programfile, list);	
		fclose(programfile);
	}
	else{
		//pass to process_input with stdin as FILE*
		process_input(stdin, list);	
	}
	//free the memory from the list
	destroy_list(list);
}
