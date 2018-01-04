/// file: brainfuck_utils.h
/// description: supporting functions header for the brainfuck language
/// @author Nicholas Jones ncj4861

#include <stdbool.h>

extern bool ZERO_NEWLINE;

#ifdef _OPS_
char* valid_ops = "<>[]+-.,";
#endif

#ifndef _UTILS_
#define _UTILS_


#include <stdio.h>
#include "cells.h"

#ifndef _UTILS_H_
#define _UTILS_H_

typedef enum bf_type_t{
	Loop, Operator, Unknown
} type;

typedef struct bf_oper_t{
	void (*operation)(LinkedList*);
} bf_oper;

union bf_union_t;

typedef struct bf_container_t{
	union  bf_union_t* operators;
	type type;
} bf_container;

typedef struct bf_loop_t{
	bf_container** oper_list; 
	unsigned current_item;
	unsigned loop_cap;
	bool complete;
} bf_list;

typedef union bf_union_t{
	bf_oper* op;
	bf_list* loop;
} bf_union;

#endif

void process_line(char* line, LinkedList* list);

void process_input(FILE* file, LinkedList* list);

void process_loop(bf_list* cur_loop_dat, LinkedList* list);

#endif
