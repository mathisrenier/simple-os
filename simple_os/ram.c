//
//  ram.c
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#include "ram.h"

// - - - - - - - - - - PRIVATE - - - - - - - - - -

char * ram[1000];
int idx = 0;    // first free index

// - - - - - - - - - - - - - - - - - - - - - - - -

void resetRam(void) {
    idx = 0;
    for(int i=0; i<1000; i++) {
        ram[i] = NULL;
    }
}


void addToRAM(FILE * p, int * start, int * end) {
    char line[100];
    * start = idx;
    
    while(fgets(line, 1000, p)) {
        if(idx < 1000) {
            ram[idx] = strdup(line);
            idx++;
        }
        else {
            // not enough space in RAM
            printf("Error: Not enough RAM to add program\n");
            * end = -1;
            unloadRAM(* start, * end);
            break;
        }
    }
    * end = idx - 1;
}


void unloadRAM(int start, int end) {
    for(int i = start; i <= end; i++) {
        ram[i] = NULL;
    }
}

char * getRAMCell(int i) {
    return ram[i];
}
