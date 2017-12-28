/// file: cells.h
/// description: abstract data type header to hold cells for Brainfuck
/// @author Nicholas Jones dojoman19@gmail.com 

#ifndef _CELLS_H_
#define _CELLS_H_

#define RESIZE_FACTOR 100
#define INITIAL_CAPACITY 30000

#ifndef _CELLS_IMPL_
#define _CELLS_IMPL_

typedef struct Node_t{
	struct Node_t* prev;
	struct Node_t* next;
	long data;
}Node;

typedef struct LinkedL_T{
	Node* head;
	unsigned capacity;
	unsigned size;
}LinkedList;

#endif

LinkedList* create_cells();

Node* get_head(LinkedList* list);

void resize_list(LinkedList* list);

void reset_list(LinkedList* list);

#endif
