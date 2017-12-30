/// file: brainfuck_utils.c
/// description: supporting functions for the brainfuck language
/// @author Nicholas Jones dojoman19@gmail.com

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cells.h"

#define _UTILS_

char* loop_dat;
unsigned int left_bracket_count = 0;

void process_line(char* line, LinkedList* list){
	for(size_t i = 0; i < strlen(line); i++){
		char c = line[i];
		switch(c){
			case '<':
				list->curr_ind--;
				break;
			case '>':
				if(list->curr_ind == list->capacity-1){
					resize_list(list);
				}
				list->curr_ind++;
				break;
			case '+':
				++*list->cells[list->curr_ind];
				break;
			case '-':
				--*list->cells[list->curr_ind];
				break;
			case '.':
				putchar(*list->cells[list->curr_ind]);
				break;
		}
		if(c == '['){
			char* nested_loop;
			int curr_i;
			if(loop_dat == NULL){
				loop_dat = calloc(strlen(line), sizeof(char));
				curr_i = i+1;
				left_bracket_count++;
				while(left_bracket_count != 0){
					if(line[curr_i] == ']')
						left_bracket_count--;
					if(line[curr_i] == '[')
						left_bracket_count++;
					if(!left_bracket_count)
						break;
					strncat(loop_dat, &line[curr_i], 1);
					curr_i++;
				}
				while(*list->cells[list->curr_ind]){
					process_line(loop_dat, list);
				}
				loop_dat = NULL;
			}
			else{
				nested_loop = calloc(strlen(line), sizeof(char));
				curr_i = i+1;
				left_bracket_count++;
				while(left_bracket_count != 0){
					if(line[curr_i] == ']')
						left_bracket_count--;
					if(line[curr_i] == '[')
						left_bracket_count++;
					if(!left_bracket_count)
						break;
					strncat(nested_loop, &line[curr_i], 1);
					curr_i++;
				}
				while(*list->cells[list->curr_ind]){
					process_line(nested_loop, list);
				}
			}
			i = curr_i;
		}
	}
}

void process_input(FILE* file, LinkedList* list){
	size_t len;
	char* line = NULL;
	if(file == stdin)
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
