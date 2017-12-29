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
				list->size--;
				//printf("Moving to cell %d\n", list->size);
				break;
			case '>':
				if(list->current->next == NULL){
					resize_list(list);
				}
				list->current = list->current->next;
				list->size++;
				//printf("Moving to cell %d\n", list->size);
				break;
			case '+':
				list->current->data++;
				break;
			case '-':
				list->current->data--;
				break;
			case '.':
				//printf("Printing! ");
				printf("%c", (char)list->current->data);
				break;
			case '[':
				loop_dat = malloc(strlen(line));
				int curr_i = i+1;
				while(line[curr_i] != ']'){
					strncat(loop_dat, &line[curr_i], 1);
					curr_i++;
				}
				int start_ind = list->size;
				//printf("Starting loop at index %d w/ data: %s\n",
				//		start_ind, loop_dat);
				long* entrance_data = &list->current->data;
				while(*entrance_data){
				//	printf("Still looping - data is %ld\n", *entrance_data);
					process_line(loop_dat, list);			
				}
				//printf("Exiting loop\n");
				//printf("Index before loop: %lu, adding: %lu\n", i, strlen(loop_dat));
				i = curr_i;			
				//printf("Current index: %lu, total length: %lu\n", i, strlen(line));
		}
	}
}

void process_input(FILE* file, LinkedList* list){
	size_t len;
	char* line = NULL;
	printf("> ");
	while(getline(&line, &len, file) != -1){
		process_line(line, list);
		if(file == stdin){
			printf("\n");
			printf("> ");
		}
	}
	free(line);
}
