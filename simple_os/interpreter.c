//
//  interpreter.c
//  COMP310-A1
//
//  Created by Mathis Renier on 2021-01-26.
//

#include "interpreter.h"


// - - - - - - - - - - PRIVATE FUNCTIONS - - - - - - - - - -

void help() {
    printf("COMMAND\t\t\t\tDESCRIPTION\n");
    printf("help\t\t\t\tDisplays all the commands\n");
    printf("quit\t\t\t\tExits/terminates the shell\n");
    printf("set VAR STRING\t\t\tAssign a value to shell memory\n");
    printf("print VAR\t\t\tDisplays the STRING assigned to VAR\n");
    printf("run SCRIPT.TXT\t\t\tExecutes the file SCRIPT.TXT\n\n");
    printf("Please note the commands are case sensitive.\n");
}


void quit() {
    printf("Bye!\n");
    exit(0);
}


void set(char * var, char * value) {
    setVar(var, value);
}


void print(char * var) {
    char * value = getVar(var);
    printf("%s\n", value);
}


void runCommand(char * fileName) {
    FILE * file = fopen(fileName, "r");
    char line[1000];
    char * args[MAX_ARGS_NUM];
    clean(args);
    
    if(!file) {
        printf("Error opening %s\n", fileName);
        return;
    }
    while(fgets(line, 1000, file)) {
        parse(line, args);
        interpreter(args);
        clean(args);
    }
    fclose(file);
}


void exec(char * fileNames[], int numArgs) {
    resetRam();
    
    // launcher
    for(int i=0; i < numArgs; i++) {
        FILE * file = fopen(fileNames[i], "r");
        if(!file) {
            printf("Error: Script %s could not be loaded\n", fileNames[i]);
            return;
        }
        
        //add i to the filename to avoid duplicates
        char s[100];
        snprintf(s, sizeof(s), "%s_%d.txt", fileNames[i], i);
        
        if(!laucher(file, s))
            printf("Error: File %s could not be lauched\n", s);
    }
    
    scheduler();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


void interpreter(char * args[]) {
    if(args[0]) {
        if(!strcmp(args[0], "help")) {
            if(args[1]) printf("help: too many arguments\n");
            else help();
        }
        else if(!strcmp(args[0], "quit")) {
            if(args[1]) printf("quit: too many arguments\n");
            else quit();
        }
        else if(!strcmp(args[0], "set")) {
            if(!(args[1] && args[2])) printf("set: too few arguments\n");
            else if(MAX_ARGS_NUM > 3 && args[3]) printf("set: too many arguments\n");
            else set(args[1], args[2]);
        }
        else if(!strcmp(args[0], "print")) {
            if(!args[1]) printf("print: too few arguments\n");
            else if(args[2]) printf("print: too many arguments\n");
            else print(args[1]);
        }
        else if(!strcmp(args[0], "run")) {
            if(!args[1]) printf("run: too few arguments\n");
            else if(args[2]) printf("run: too many arguments\n");
            else runCommand(args[1]);
        }
        else if(!strcmp(args[0], "exec")) {
            if(!args[1]) printf("exec: too few arguments\n");
            else if(args[4]) printf("exec: too many arguments\n");
            else if(!args[2]) exec(args+1, 1);
            else if(args[2] && !args[3]) exec(args+1, 2);
            else exec(args + 1, 3);
        }
        else printf("Unknown command %s\n", args[0]);
    }
}

void parse(char * input, char * args[]) {
    char ** arg = args;
    char * token;

    *arg++ = strtok(input, " \r\n\t");
    while((token = strtok(NULL, " \r\n\t"))) {
        *arg++ = token;
    }
}

void clean(char * args[]) {
    for(int i = 0; i < MAX_ARGS_NUM; i++) {
        args[i] = NULL;
    }
}
