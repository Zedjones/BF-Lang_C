/// file: brainfuck_utils.c
/// description: supporting functions for the brainfuck language
/// @author Nicholas Jones dojoman19@gmail.com

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cells.h"

#define _UTILS_

void process_line(char* line, LinkedList* list){
	for(size_t i = 0; i < strlen(line); i++){
		char c = line[i];
		char* loop_dat;
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
				printf("Printing! ");
				printf("%c", (char)list->current->data);
				break;
			case '[':
				loop_dat = strtok(&line[i+1], "]");
				long* entrance_data = &list->current->data;
				while(*entrance_data){
					process_line(loop_dat, list);			
				}
				i += strlen(loop_dat);			
				break;
		}
	}
}

void process_input(FILE* file, LinkedList* list){
	size_t len;
	char* line = NULL;
	printf("> ");
	while(getline(&line, &len, file) != -1){
		process_line(line, list);
		printf("\n");
		printf("> ");
	}
	free(line);
}
