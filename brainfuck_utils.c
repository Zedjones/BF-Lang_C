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

bf_list* main_loop;
unsigned current_ind;
bool incomplete_loop = false; 
bool ZERO_NEWLINE;

#define _UTILS_

void func_move_l(LinkedList* list){
	if(list->curr_ind == 0){
		list->curr_ind = list->capacity-1;
		return;
	}
	list->curr_ind--;
}

void func_move_r(LinkedList* list){
	if(list->curr_ind == list->capacity-1)
		resize_list(list);
	if(list->curr_ind == list->capacity-1){
		list->curr_ind = 0;
		return;
	}
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

void free_loop(bf_list* loop){
	for(unsigned i = 0; i < loop->current_item; i++){
		bf_container* current = loop->oper_list[i];
		if(current->type == Operator){
			free(current->operators->op);
			free(current->operators);
			free(current);
		}
		else if(current->type == Loop){
			free_loop(current->operators->loop);
			free(current->operators);
			free(current);
		}
	}
	free(loop->oper_list);
	free(loop);
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
			free(container->operators);
			free(container);
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
				free_loop(main_loop);
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
