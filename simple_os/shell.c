//
//  shell.c
//  COMP310-A1
//
//  Created by Mathis Renier on 2021-01-26.
//

#include "shell.h"


int shellUI(void) {
    char input[MAX_INPUT_LENGTH];
    char * args[MAX_ARGS_NUM];
    clean(args);
    
    printf("Welcome to the Mathis shell!\nVersion 2.0 Updated February 2021\n");
    
    while(1) {
        if(isatty(STDIN_FILENO)) printf("$ ");  // avoids printing $ when redirecting the input
        
        fgets(input, MAX_INPUT_LENGTH, stdin);
        
        parse(input, args);
        interpreter(args);
        clean(args);
    }
}
