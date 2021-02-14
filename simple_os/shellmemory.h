//
//  shellmemory.h
//  COMP310-A1
//
//  Created by Mathis Renier on 2021-01-26.
//

#ifndef shellmemory_h
#define shellmemory_h

#include <stdio.h>
#include <string.h>

#define MEMORY_SIZE 1000

void setVar(char * var, char * value);
char * getVar(char * var);

#endif /* shellmemory_h */
