/// file: brainfuck_utils.h
/// description: supporting functions header for the brainfuck language
/// @author Nicholas Jones ncj4861

#include <stdbool.h>

///shared variable to keep track of whether or not 
///we treat newlines as 0 
extern bool ZERO_NEWLINE;

#ifdef _OPS_
///list of valid operators
char* valid_ops = "<>[]+-.,";
#endif

#ifndef _UTILS_
#define _UTILS_

#include <stdio.h>
#include "cells.h"

#ifndef _UTILS_H_
#define _UTILS_H_

///enum for possible operation types
typedef enum bf_type_t{
	Loop, Operator, Unknown
} type;

///operator that holds a function pointer
typedef struct bf_oper_t{
	void (*operation)(LinkedList*);
} bf_oper;

///declaration of bf_union_t
union bf_union_t;

///a container that hold a bf_union and a type
typedef struct bf_container_t{
	union  bf_union_t* operators;
	type type;
} bf_container;

///loop construct to hold a list of containers 
///(meaning a loop can contain another loop), 
///as well as other information
typedef struct bf_loop_t{
	bf_container** oper_list; 
	unsigned current_item;
	unsigned loop_cap;
	bool complete;
} bf_list;

///a union that can hold either an operator or a loop
typedef union bf_union_t{
	bf_oper* op;
	bf_list* loop;
} bf_union;

#endif

void process_line(char* line, LinkedList* list);

void process_input(FILE* file, LinkedList* list);

void process_loop(bf_list* cur_loop_dat, LinkedList* list);

#endif
