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

typedef enum oper_type{
	LEFT, RIGHT, INC, DEC
} operation;

#endif

void process_line(char* line, LinkedList* list);

void process_input(FILE* file, LinkedList* list);

#endif
