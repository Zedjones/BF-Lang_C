/// file: cells.c
/// description: abstract data type to hold cells for Brainfuck
/// @author Nicholas Jones dojoman19@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include "cells.h"

#define _CELLS_IMPL_

LinkedList* create_cells(){
	LinkedList* list = malloc(sizeof(LinkedList));
	list->cells = malloc(sizeof(long*)*INITIAL_CAPACITY);
	if(list->cells == NULL)
		fprintf(stderr, "Cannot allocate storage for cells.");
	for(unsigned i = 0; i < INITIAL_CAPACITY; i++){
		list->cells[i] = calloc(1, sizeof(long*));
	}
	list->curr_ind = 0;
	list->capacity = INITIAL_CAPACITY;
	return list;
}

void resize_list(LinkedList* list){
	unsigned start_cap = list->capacity;
	unsigned start_ind = list->curr_ind;
	list->cells = realloc(list->cells, sizeof(long*) * 
			(start_cap + RESIZE_FACTOR));
	for(unsigned i = start_ind+1; i <= start_ind+RESIZE_FACTOR; i++){
		list->cells[i] = calloc(1, sizeof(long*));
	}
	list->capacity += RESIZE_FACTOR;
}

void reset_list(LinkedList* list){
	for(unsigned i = 0; i < list->capacity; i++){
		*list->cells[i] = 0;
	}
	list->curr_ind = 0;
}

void destroy_list(LinkedList* list){
	for(unsigned i = 0; i < list->capacity; i++){
		free(list->cells[i]);
	}
	free(list->cells);
	free(list);
}
