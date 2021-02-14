//
//  shellmemory.c
//  COMP310-A1
//
//  Created by Mathis Renier on 2021-01-26.
//

#include "shellmemory.h"


// - - - - - - - - - - PRIVATE - - - - - - - - - -

struct Memory{
    char var[100];
    char value[100];
} shellMemory[MEMORY_SIZE];

int memIndex = 0;

// - - - - - - - - - - - - - - - - - - - - - - - -

void setVar(char * var, char * value) {
    // update existing variable
    for(int i = 0; i < memIndex && memIndex <= MEMORY_SIZE; i++) {
        if(!strcmp(shellMemory[i].var, var)) {
            if(memIndex == MEMORY_SIZE) printf("Warning: shell memory is full\n");
            strcpy(shellMemory[i].value, value);
            return;
        }
    }
    // create new variable
    if(memIndex >= MEMORY_SIZE) {
        printf("Error: shell memory is full\n");
        return;
    }
    strcpy(shellMemory[memIndex].var, var);
    strcpy(shellMemory[memIndex].value, value);
    memIndex++;
}


char * getVar(char * var) {
    for(int i = 0; i < memIndex && memIndex <= MEMORY_SIZE; i++) {
        if(!strcmp(shellMemory[i].var, var)) {
            return shellMemory[i].value;
        }
    }
    return "Variable does not exist";
}
