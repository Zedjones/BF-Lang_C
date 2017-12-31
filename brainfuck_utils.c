/// file: brainfuck_utils.c
/// description: supporting functions for the brainfuck language
/// @author Nicholas Jones dojoman19@gmail.com

#define _GNU_SOURCE
#define _OPS_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cells.h"
#include "brainfuck_utils.h"

char* loop_dat;
unsigned short loop_dat_len;
unsigned short left_bracket_count = 0;
bool incomplete_loop = false; 
bool ZERO_NEWLINE;

#define _UTILS_

void process_line(char* line, LinkedList* list){
	for(size_t i = 0; i < strlen(line); i++){
		char c = line[i];
		if(c == '[' || incomplete_loop){
			char* nested_loop;
			unsigned curr_i;
			if(loop_dat == NULL || incomplete_loop){
				if(loop_dat == NULL){
					loop_dat = calloc(strlen(line), sizeof(char*));
					loop_dat_len = strlen(line);
					left_bracket_count++;
					curr_i = i+1;
				}
				else{
					loop_dat = realloc(loop_dat, loop_dat_len+strlen(line) * 
							sizeof(char*));
					loop_dat_len += strlen(line);
					curr_i = i;
				}
				while(true){
					if(curr_i == strlen(line)-1){
						incomplete_loop = true;
						break;
					}
					else if(line[curr_i] == ']'){
						left_bracket_count--;
					}
					else if(line[curr_i] == '['){
						left_bracket_count++;
					}
					if(!left_bracket_count){
						incomplete_loop = false;	
						break;
					}
					if(strchr(valid_ops, line[curr_i])){
						strncat(loop_dat, &line[curr_i], 1);
					}
					curr_i++;
				}
				if(incomplete_loop){
					i = curr_i;
					continue;
				}
				while(*list->cells[list->curr_ind]){
					process_line(loop_dat, list);
				}
				free(loop_dat);
				loop_dat = NULL;
			}
			else{
				nested_loop = calloc(strlen(line), sizeof(char*));
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
				free(nested_loop);
			}
			i = curr_i;
		}
		char temp;
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
			case ',':
				temp = getchar();
				if(ZERO_NEWLINE && temp == '\n'){
					*list->cells[list->curr_ind] = 0;
				}
				else
					*list->cells[list->curr_ind] = temp;
				break;
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
			if(incomplete_loop)
				printf("... ");
			else
				printf("> ");
		}
	}
	free(line);
}
