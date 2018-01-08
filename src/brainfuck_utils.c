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

//main loop, which may span more than one line
bf_list* main_loop;
//current index of the current line
unsigned current_ind;
//whether or not the current loop is complete
bool incomplete_loop = false; 
//whether or not to interpret newline as 0
bool ZERO_NEWLINE;
//whether or not to immediately flush to stdout
bool FLUSH;

#define _UTILS_

/**
Function to move one cell to the left
@param list - list of cells
*/
void func_move_l(LinkedList* list){
	//if we are on the left-most index, wrap around
	if(list->curr_ind == 0){
		list->curr_ind = list->capacity-1;
		return;
	}
	list->curr_ind--;
}

/**
Function to move one cell to the right
@param list - list of cells
*/
void func_move_r(LinkedList* list){
	//if we are on the right-most index, add more cells
	if(list->curr_ind == list->capacity-1)
		resize_list(list);
	list->curr_ind++;
}

/**
Function to increment value of current cell
@param list - list of cells
*/
void func_inc(LinkedList* list){
	++list->cells[list->curr_ind];
}

/**
Function to decrement value of current cell
@param list - list of cells
*/
void func_dec(LinkedList* list){
	--list->cells[list->curr_ind];
}

/**
Function to read input and set value of current cell to input
@param list - list of cells
*/
void func_read(LinkedList* list){
	char temp;
	//use read for faster reading
	char size = read(0, &temp, 1);
	(void)size;
	if(ZERO_NEWLINE && temp == '\n')
		list->cells[list->curr_ind] = 0;
	//no change on EOF
	else if(temp == EOF)
		return;
	else
		list->cells[list->curr_ind] = temp;
}

/**
Function to write the value of the current cell to stdout
@param list - list of cells
*/
void func_write(LinkedList* list){
	//char size = write(1, &list->cells[list->curr_ind], 1);
	//(void)size;
	putchar(list->cells[list->curr_ind]);
	if(FLUSH){
		fflush(stdout);
	}
}

/**
Frees all allocated memory from a bf_list
@param loop - bf_list to free
*/
void free_loop(bf_list* loop){
	for(unsigned i = 0; i < loop->current_item; i++){
		bf_container* current = loop->oper_list[i];
		if(current->type == Operator){
			//free bf_oper and bf_union
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
	//free list itself
	free(loop->oper_list);
	free(loop);
}

/**
Print each item in the loop 
@param loop - the loop to print
*/
void print_loop(bf_list* loop){
	printf("[");
	for(unsigned i = 0; i < loop->current_item; i++){
		bf_container* current = loop->oper_list[i];
		//print symbol based on operation function
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
		//recurse if it's a loop
		else if(current->type == Loop){
			print_loop(current->operators->loop);
		}
		//shouldn't ever reach this
		else{
			printf(" Unknown ");
		}
	}
	//if the loop is complete, print a C]
	if(loop->complete){
		printf("C]");
	}
}

/**
Run the loop for a bf_list
@param loop - the loop to run 
@param list - the list of cells
*/
void process_loop(bf_list* loop, LinkedList* list){
	//while the current cell's value isn't 0
	while(list->cells[list->curr_ind]){
		//for each item in the loop
		for(unsigned i = 0; i < loop->current_item; i++){
			bf_container* container = loop->oper_list[i];
			//if the element is an operation, call function
			if(container->type == Operator){
				container->operators->op->operation(list);
			}
			//otherwise, recurse
			else{
				process_loop(container->operators->loop, list);
			}
		}
	}
}

/**
Creates a loop given a loop (or NULL/pointer to NULL) and a line 
@param list - the loop to add to (or NULL)
@param line - the line to process into the loop
*/
bf_list* create_loop(bf_list* list, char* line){
	//if pointer to NULL or NULL
	if(list == NULL){
		//allocate storage for the loop and the bf_container list
		list = malloc(sizeof(bf_list));
		list->oper_list = malloc(sizeof(bf_container)*strlen(line));
		//set initial values
		list->current_item = 0;
		list->loop_cap = strlen(line);
		list->complete = false;
	}
	else{
		//reallocate the operator list so that it's guaranteed to 
		//hold all necessary data 
		list->oper_list = realloc(list->oper_list, sizeof(bf_container) * 
				(list->loop_cap + strlen(line)));
		list->loop_cap += strlen(line); //set new capacity
		//if the loop has any items
		if(list->current_item > 0){
			//get the last item in the list
			bf_container* current = list->oper_list[list->current_item-1];
			//if it's a loop
			if(current->type == Loop){
				bf_list* inner_loop = current->operators->loop;		
				//if it's not complete, then recurse 
				if(!inner_loop->complete){
					inner_loop = create_loop(inner_loop, line);			
					//add one to the current index (for the ]), but only if
					//loop has completed
					if(inner_loop->complete)
						current_ind++;
				}
			}
		}
	}
	//process the rest of the line
	while(current_ind < strlen(line)){
		char c = line[current_ind];
		//allocate storage for the container and union
		bf_container* container = malloc(sizeof(bf_container));
		container->operators = malloc(sizeof(bf_union));
		bf_union* operators = container->operators;
		//assume the container type is unknown (this would happen if 
		//we get an unexpected character)
		container->type = Unknown;
		//set operation for basic functions
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
			//increment index before we enter loop so we don't get 
			//stuck in an infinite loop
			current_ind++;
			//since it's a new loop, use NULL and the line
			operators->loop = create_loop(NULL, line);
			//set type to loop
			container->type = Loop;
		}
		else if(c == ']'){
			//free allocated memory since we won't add this container
			free(container->operators);
			free(container);
			//set the list to complete since we've hit a right bracket
			//and then return the list
			list->complete = true;
			return list;
		}
		//only add container if we've set a different type
		if(container->type != Unknown){
			list->oper_list[list->current_item++] = container;
		}
		current_ind++;
	}
	//if we hit the end without a right bracket, list is not 
	//complete and then we return
	list->complete = false;
	return list;
}

/**
Process and tokenize a line
@param line - the list of characters to process
@param list - the list of cells 
*/
void process_line(char* line, LinkedList* list){
	//reset current_ind from last line 
	for(current_ind = 0; current_ind < strlen(line); current_ind++){
		char c = line[current_ind];
		//if we hit a left bracket or our loop is incomplete
		if(c == '[' || incomplete_loop){
			//if it's the initial loop
			if(line[current_ind] == '[' && !incomplete_loop){
				//increment current index and create the loop
				current_ind++;
				main_loop = create_loop(main_loop, line);
			}
			else{
				main_loop = create_loop(main_loop, line);
			}
			//if the main loop is not complete, set incomplete_loop
			if(!main_loop->complete){
				incomplete_loop = true;
			}
			else{
				incomplete_loop = false;
			}
			//if the loop is complete
			if(!incomplete_loop){
				//process the loop and then free it and reset main_loop
				process_loop(main_loop, list);
				free_loop(main_loop);
				main_loop = NULL;
			}
			continue;
		}
		//get character again in case it changed in loop processing
		//c = line[current_ind];
		//Since non-loop commands only run once, we can process them
		//as characters
		switch(c){
			//run corresponding function based on character
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

/**
Process the input from the file with the list of cells
@param - file to read program from
@param - the list of cells 
*/
void process_input(FILE* file, LinkedList* list){
	size_t len;
	char* line = NULL;
	//only print prompt if in interactive mode
	if(file == stdin)
		printf("> ");
	//while there are lines to process
	while(getline(&line, &len, file) != -1){
		//process the line 
		process_line(line, list);
		//if in interactive mode
		if(file == stdin){
			printf("\n");
			//print dots for incomplete loop
			if(incomplete_loop)
				printf("... ");
			else
				printf("> ");
		}
	}
	//free line at the end
	free(line);
}
