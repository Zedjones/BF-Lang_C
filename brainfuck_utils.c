/// file: brainfuck_utils.c
/// description: supporting functions for the brainfuck language
/// @author Nicholas Jones dojoman19@gmail.com

#define _GNU_SOURCE
#define _OPS_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cells.h"
#include "brainfuck_utils.h"

char* loop_dat;
bf_list* main_loop;
unsigned short loop_dat_len;
unsigned short left_bracket_count = 0;
bool incomplete_loop = false; 
bool ZERO_NEWLINE;

#define _UTILS_

void func_move_l(LinkedList* list){
	list->curr_ind--;
}

void func_move_r(LinkedList* list){
	if(list->curr_ind == list->capacity-1)
		resize_list(list);
	list->curr_ind++;
}

void func_inc(LinkedList* list){
	++list->cells[list->curr_ind];
}

void func_dec(LinkedList* list){
	--list->cells[list->curr_ind];
}

void func_read(LinkedList* list){
	char temp;
	read(0, &temp, 1);
	if(ZERO_NEWLINE && temp == '\n')
		list->cells[list->curr_ind] = 0;
	else if(temp == EOF)
		return;
	else
		list->cells[list->curr_ind] = temp;
}

void func_write(LinkedList* list){
	write(1, &list->cells[list->curr_ind], 1);
}

void process_loop(bf_list* loop, LinkedList* list){
	for(unsigned i = 0; i < loop->loop_size; i++){
		bf_container* container = loop->oper_list[i];
		if(container->type == Operator){
			container->operators->op->operation(list);
		}
		else{
			process_loop(container->operators->loop, list);
		}
	}
}

bf_list* create_list(char* line){
	bf_list* main_list = malloc(sizeof(bf_list));
	main_list->current_item = 0;
	main_list->loop_size = 0;
	for(size_t i = 0; i < strlen(line); i++){
		char c = line[i];
		bf_container* container = malloc(sizeof(bf_container));
		if(c == '>')
			container->operators->op->operation = func_move_r;
		else if(c == '<')
			container->operators->op->operation = func_move_l;
		else if(c == '-')
			container->operators->op->operation = func_dec;
		else if(c == '+')
			container->operators->op->operation = func_inc;
		else if(c == ',')
			container->operators->op->operation = func_read;
		else if(c == '.')
			container->operators->op->operation = func_write;
		else if(c == '['){
			container->operators->loop = create_list(&line[i]);
		}
		else if(c == ']'){
			return main_list;
		}
		main_list->oper_list[main_list->current_item] = container;
		main_list->current_item++;
		main_list->loop_size++;
	}
	return main_list;
}

void process_line(char* line, LinkedList* list){
	for(size_t i = 0; i < strlen(line); i++){
		char c = line[i];
		if(c == '[' || incomplete_loop){
			char* nested_loop;
			unsigned curr_i;
			if(loop_dat == NULL || incomplete_loop){
				if(loop_dat == NULL){
					loop_dat = calloc(strlen(line), sizeof(char));
					loop_dat_len = strlen(line);
					left_bracket_count++;
					curr_i = i+1;
				}
				else{
					loop_dat = realloc(loop_dat, loop_dat_len+strlen(line) * 
							sizeof(char));
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
				while(list->cells[list->curr_ind]){
					process_line(loop_dat, list);
				}
				free(loop_dat);
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
				while(list->cells[list->curr_ind]){
					process_line(nested_loop, list);
				}
				free(nested_loop);
			}
			i = curr_i;
		}
		//char temp;
		switch(c){
			case '<':
				func_move_l(list);
				break;
			case '>':
				/*
				if(list->curr_ind == list->capacity-1){
					resize_list(list);
				}
				list->curr_ind++;
				*/
				func_move_r(list);
				break;
			case '+':
				//++list->cells[list->curr_ind];
				func_inc(list);
				break;
			case '-':
				//--list->cells[list->curr_ind];
				func_dec(list);
				break;
			case '.':
				//write(1, &list->cells[list->curr_ind], 1);
				func_write(list);
				break;
			case ',':
				/*
				temp = getchar();
				if(ZERO_NEWLINE && temp == '\n')
					list->cells[list->curr_ind] = 0;
				else if(temp == EOF)
					break;
				else
					list->cells[list->curr_ind] = temp;
				*/
				func_read(list);
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
