/// file: cells.c
/// description: abstract data type to hold cells for Brainfuck
/// @author Nicholas Jones dojoman19@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include "cells.h"

#define _CELLS_IMPL_

LinkedList* create_cells(){
	LinkedList* list = malloc(sizeof(LinkedList));
	list->head = malloc(sizeof(Node));
	list->head->data = 0;
	Node* current = list->head;
	for(int i = 0; i < INITIAL_CAPACITY-1; i++){
		current->next = malloc(sizeof(Node));
		current->next->data = 0;
		current->next->prev = current;
		current = current->next;	
	}
	list->size = 0;
	list->capacity = INITIAL_CAPACITY;
	return list;
}

Node* get_head(LinkedList* list){
	return list->head;
}

void resize_list(LinkedList* list){
	Node* current = get_head(list);
	while(current->next != NULL){
		current = current->next;
	}
	for(int i = 0; i < RESIZE_FACTOR; i++){
		current->next = malloc(sizeof(Node));
		current->next->data = 0;
		current->next->prev = current;
		current = current->next;
	}
	list->capacity += RESIZE_FACTOR;
}

void reset_list(LinkedList* list){
	Node* current = get_head(list);
	while(current->next != NULL){
		current->data = 0;
		current = current->next;
	}
	list->size = 0;
}
