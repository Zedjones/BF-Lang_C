/// file: cells.h
/// description: abstract data type header to hold cells for Brainfuck
/// @author Nicholas Jones dojoman19@gmail.com 

#ifndef _CELLS_H_
#define _CELLS_H_

#define RESIZE_FACTOR 30000
#define INITIAL_CAPACITY 30000

#ifndef _CELLS_IMPL_
#define _CELLS_IMPL_

typedef struct LinkedL_T{
	long** cells;
	unsigned capacity;
	unsigned curr_ind;
} LinkedList;

#endif

LinkedList* create_cells();

void resize_list(LinkedList* list);

void reset_list(LinkedList* list);

void destroy_list(LinkedList* list);

#endif
