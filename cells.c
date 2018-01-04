/// file: cells.c
/// description: abstract data type to hold cells for Brainfuck
/// @author Nicholas Jones dojoman19@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include "cells.h"

#define _CELLS_IMPL_

/**
Create a list of cells 
@return list - a new LinkedList of cells 
*/
LinkedList* create_cells(){
	//allocate memory
	LinkedList* list = malloc(sizeof(LinkedList));
	list->cells = malloc(sizeof(char)*INITIAL_CAPACITY);
	//if storage cannot be allocated, return an error
	if(list->cells == NULL)
		fprintf(stderr, "Cannot allocate storage for cells.");
	//set all initial cell values to 0
	for(unsigned i = 0; i < INITIAL_CAPACITY; i++){
		list->cells[i] = 0;
	}
	//set initial info
	list->curr_ind = 0;
	list->capacity = INITIAL_CAPACITY;
	return list;
}

/**
Resize a list to hold current capacity + RESIZE_FACTOR number of cells
@param list - the list to resize
*/
void resize_list(LinkedList* list){
	unsigned start_cap = list->capacity;
	unsigned start_ind = list->curr_ind;
	list->cells = realloc(list->cells, sizeof(char) * 
			(start_cap + RESIZE_FACTOR));
	//initialize the new cells
	for(unsigned i = start_ind+1; i <= start_ind+RESIZE_FACTOR; i++){
		list->cells[i] = 0;
	}
	//update capacity
	list->capacity += RESIZE_FACTOR;
}

/**
Reset the value of each cell in the list 
@param list - the list to reset
*/
void reset_list(LinkedList* list){
	for(unsigned i = 0; i < list->capacity; i++){
		list->cells[i] = 0;
	}
	list->curr_ind = 0;
}

/**
Free dynamically allocated memory in the list
@param list - LinkedList to free
*/
void destroy_list(LinkedList* list){
	free(list->cells);
	free(list);
}
