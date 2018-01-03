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

void print_loop(bf_list* loop){
	printf("Looping over %d elements\n", loop->current_item);
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
			printf("]");
		}
		else{
			printf(" Unknown ");
		}
	}
	printf("Done printing\n");
}

void process_loop(bf_list* loop, LinkedList* list){
	while(list->cells[list->curr_ind]){
		//printf("Loop current item: %u\n", loop->current_item);
		for(unsigned i = 0; i < loop->current_item; i++){
			bf_container* container = loop->oper_list[i];
			if(container->type == Operator){
				container->operators->op->operation(list);
			}
			else{
				//printf("Going through another loop\n");
				process_loop(container->operators->loop, list);
			}
		}
	}
}

bf_list* create_list(bf_list* list, char* line){
	size_t i = 0;
	if(list == NULL){
		//printf("Allocating main list\n");
		list = malloc(sizeof(bf_list));
		list->oper_list = malloc(sizeof(bf_container*) * strlen(line));
		list->current_item = 0;
		list->loop_cap = strlen(line);
		list->complete = false;
	}
	else if(!list->complete){
		//printf("Reallocating!\n");
		list->oper_list = realloc(main_loop->oper_list, sizeof(bf_container)* 
				main_loop->loop_cap + strlen(line));
		list->loop_cap += strlen(line);
		bf_container* last_item = list->oper_list[list->current_item-1];
		if(last_item->type == Loop && last_item->operators->loop->complete){
			create_list(last_item->operators->loop, line);	
		}
		unsigned curr_i = 0;
		unsigned left_brackets = 1;
		while(true){
			char temp = line[curr_i];
			if(temp == '[')
				left_brackets++;
			else if(temp == ']')
				left_brackets--;
			curr_i++;
			if(!left_brackets)
				break;
		}
		i = curr_i-1;
	}
	else{
		list = malloc(sizeof(bf_list));
		list->oper_list = malloc(sizeof(bf_container*) * strlen(line));
		list->loop_cap = strlen(line);
		list->current_item = 0;
	}
	while(i < strlen(line)){
		char c = line[i];
		//printf("Processing %c\n", c);
		bf_container* container = malloc(sizeof(bf_container));
		container->operators = malloc(sizeof(bf_union));
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
			//printf("Processing %s\n", &line[i+1]);
			container->operators->loop = create_list(NULL, &line[i+1]);
			//printf("Done processing %s\n", &line[i+1]);
			unsigned curr_i = i+1;
			unsigned left_brackets = 1;
			//printf("Original curr_i: %d\n", curr_i);
			while(true){
				char temp = line[curr_i];
				if(temp == '[')
					left_brackets++;
				else if(temp == ']')
					left_brackets--;
				curr_i++;
				if(!left_brackets)
					break;
				if(curr_i == strlen(line)-1){
					list->complete = false;
					return list;
				}
			}
			//printf("Setting i to %d\n", curr_i);
			//printf("Next character: %c\n", line[curr_i]);
			i = curr_i-1;
			container->type = Loop;
		}
		else if(c == ']'){
			//printf("Exiting loop construction\n");
			free(container);
			//printf("Final loop: ");
			//print_loop(list);
			//printf("\n");
			incomplete_loop = false;
			list->complete = true;
			return list;
		}
		else{
			free(container);
			i++;
			continue;
		}
		list->oper_list[list->current_item] = container;
		list->current_item++;
		i++;
	}
	//printf("Loop processing complete.");
	list->complete = false;
	incomplete_loop = true;
	return list;
}

void process_line(char* line, LinkedList* list){
	for(size_t i = 0; i < strlen(line); i++){
		char c = line[i];
		if(c == '[' || incomplete_loop){
			/*
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
			*/
			//printf("Creating main loop.\n");
			main_loop = create_list(main_loop, &line[i+1]);
			unsigned curr_i = i+1;
			unsigned left_brackets = 1;
			while(left_brackets){
				char curr = line[curr_i];
				if(curr == '[')
					left_brackets++;
				else if(curr == ']')
					left_brackets--;
				curr_i++;
				if(!left_brackets)
					break;
				if(i == strlen(line)-1){
					incomplete_loop = true;
					break;
				}
			}
			if(incomplete_loop){
				i = curr_i-1;
				continue;
			}
			incomplete_loop = false;
			process_loop(main_loop, list);
			main_loop = NULL;
			//printf("After loop, on index %d\n", curr_i-1);
			//printf("Next character is %c\n", line[curr_i]);
			i = curr_i-1;
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
