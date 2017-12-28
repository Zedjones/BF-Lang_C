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
	return list;
}

Node* get_head(LinkedList* list){
	return list->head;
}

int main(){
	return 0;
}
