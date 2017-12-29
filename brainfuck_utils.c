/// file: brainfuck_utils.c
/// description: supporting functions for the brainfuck language
/// @author Nicholas Jones dojoman19@gmail.com

#include <stdio.h>
#include <string.h>
#include "cells.h"

#define _UTILS_

void process_line(char* line, LinkedList* list){
	for(size_t i = 0; i < strlen(line); i++){
		char c = line[i];
		switch(c){
			case '<':
				list->current = list->current->prev;
				break;
			case '>':
				if(list->current->next == NULL){
					resize_list(list);
				}
				list->current = list->current->next;
				break;
			case '+':
				list->current->data++;
				break;
			case '-':
				list->current->data--;
				break;
			case '.':
				printf("%c", (char)list->current->data);
		}
	}
}

void process_input(FILE* file, LinkedList* list){
	char* line = NULL;
	printf("> ");
	while(getline(&line, NULL, file) != -1){
		process_line(line, list);
		printf("\n");
		printf("> ");
	}
	free(line);
}
