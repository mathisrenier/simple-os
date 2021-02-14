//
//  interpreter.h
//  COMP310-A1
//
//  Created by Mathis Renier on 2021-01-26.
//

#ifndef interpreter_h
#define interpreter_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"

#define MAX_INPUT_LENGTH 1000
#define MAX_ARGS_NUM 10

void interpreter(char * args[]);
void parse(char * input, char * args[]);
void clean(char * args[]);

#endif /* interpreter_h */
