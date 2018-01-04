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
unsigned current_ind;
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

void print_loop(bf_list* loop){
	//printf("Looping over %d elements\n", loop->current_item);
	for(unsigned i = 0; i < loop->current_item; i++){
		//printf("Processing item %d\n", i);
		bf_container* current = loop->oper_list[i];
		if(current->type == Operator){
			if(current->operators->op->operation == func_write)
				printf(".");
			else if(current->operators->op->operation == func_read)
				printf(",");
			else if(current->operators->op->operation == func_inc)
				printf("+");
			else if(current->operators->op->operation == func_dec)
				printf("-");
			else if(current->operators->op->operation == func_move_r)
				printf(">");
			else if(current->operators->op->operation == func_move_l)
				printf("<");
		}
		else if(current->type == Loop){
			printf("[");
			print_loop(current->operators->loop);
		}
		else{
			printf(" Unknown ");
		}
	}
	if(loop->complete){
		printf("C]");
	}
}

void process_loop(bf_list* loop, LinkedList* list){
	//print_loop(main_loop);
	//printf("Running loop.\n");
	while(list->cells[list->curr_ind]){
		//printf("Loop current item: %u\n", loop->current_item);
		for(unsigned i = 0; i < loop->current_item; i++){
			bf_container* container = loop->oper_list[i];
			if(container->type == Operator){
				container->operators->op->operation(list);
			}
			else{
				process_loop(container->operators->loop, list);
			}
		}
	}
}

bf_list* create_loop(bf_list* list, char* line){
	if(list == NULL){
		list = malloc(sizeof(bf_list));
		list->oper_list = malloc(sizeof(bf_container)*strlen(line));
		list->current_item = 0;
		list->loop_cap = strlen(line);
		list->complete = false;
	}
	else{
		//printf("Current loop ");
		//printf("[");
		//print_loop(list);
		//printf("\n");
		list->oper_list = realloc(list->oper_list, sizeof(bf_container) * 
				(list->loop_cap + strlen(line)));
		list->loop_cap += strlen(line);
		if(list->current_item > 0){
			bf_container* current = list->oper_list[list->current_item-1];
			if(current->type == Loop){
				bf_list* inner_loop = current->operators->loop;		
				if(!inner_loop->complete){
					inner_loop = create_loop(inner_loop, line);			
					if(inner_loop->complete)
						current_ind++;
				}
			}
		}
	}
	//printf("Creating loop from %s", &line[current_ind]);
	//printf("Current index: %d\n", current_ind);
	while(current_ind < strlen(line)){
		char c = line[current_ind];
		//printf("Processing %c\n", line[current_ind]);
		bf_container* container = malloc(sizeof(bf_container));
		container->operators = malloc(sizeof(bf_union));
		bf_union* operators = container->operators;
		container->type = Unknown;
		//printf("Processing %c\n", c);
		if(c == '>'){
			container->operators->op = malloc(sizeof(bf_oper));
			container->operators->op->operation = func_move_r;
			container->type = Operator;
		}
		else if(c == '<'){
			container->operators->op = malloc(sizeof(bf_oper));
			container->operators->op->operation = func_move_l;
			container->type = Operator;
		}
		else if(c == '-'){
			container->operators->op = malloc(sizeof(bf_oper));
			container->operators->op->operation = func_dec;
			container->type = Operator;
		}
		else if(c == '+'){
			container->operators->op = malloc(sizeof(bf_oper));
			container->operators->op->operation = func_inc;
			container->type = Operator;
		}
		else if(c == ','){
			container->operators->op = malloc(sizeof(bf_oper));
			container->operators->op->operation = func_read;
			container->type = Operator;
		}
		else if(c == '.'){
			container->operators->op = malloc(sizeof(bf_oper));
			container->operators->op->operation = func_write;
			container->type = Operator;
		}
		else if(c == '['){
			current_ind++;
			operators->loop = create_loop(NULL, line);
			container->type = Loop;
		}
		else if(c == ']'){
			//printf("Final loop ");
			//printf("[");
			//print_loop(list);
			//printf("]");
			//printf("\n");
			list->complete = true;
			return list;
		}
		if(container->type != Unknown){
			list->oper_list[list->current_item] = container;
			list->current_item++;
		}
		current_ind++;
	}
	list->complete = false;
	return list;
}

void process_line(char* line, LinkedList* list){
	for(current_ind = 0; current_ind < strlen(line); current_ind++){
		char c = line[current_ind];
		if(c == '[' || incomplete_loop){
			/*
			char* nested_loop;
			unsigned curr_i;
			if(loop_dat == NULL || incomplete_loop){
				if(loop_dat == NULL){
					loop_dat = calloc(strlen(line), sizeof(char));
					loop_dat_len = strlen(line);
					left_bracket_count++;
				printf("Done creating main loop\n");
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
			*/
			if(line[current_ind] == '[' && !incomplete_loop){
				//printf("Start condition entered\n");
				current_ind++;
				main_loop = create_loop(main_loop, line);
			}
			else{
				main_loop = create_loop(main_loop, line);
			}
			if(!main_loop->complete){
				incomplete_loop = true;	
			}
			else{
				incomplete_loop = false;
			}
			if(!incomplete_loop){
				//printf("Complete loop ");
				//print_loop(main_loop);
				//printf("\n");
				process_loop(main_loop, list);
				main_loop = NULL;
			}
		}
		switch(c){
			case '<':
				func_move_l(list);
				break;
			case '>':
				func_move_r(list);
				break;
			case '+':
				func_inc(list);
				break;
			case '-':
				func_dec(list);
				break;
			case '.':
				func_write(list);
				break;
			case ',':
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
		//printf("Processing line: %s\n", line);
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
