//
//  ram.c
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#include "ram.h"

// - - - - - - - - - - PRIVATE - - - - - - - - - -

char * ram[RAM_SIZE];
int idx = 0;    // first free index

// - - - - - - - - - - - - - - - - - - - - - - - -

void resetRam(void) {
    idx = 0;
    for(int i=0; i<RAM_SIZE; i++) {
        ram[i] = NULL;
    }
}


//void addToRAM(FILE * p, int * start, int * end) {
//    char line[100];
//    * start = idx;
//    
//    while(fgets(line, 100, p)) {
//        if(idx < RAM_SIZE) {
//            ram[idx] = strdup(line);
//            idx++;
//        }
//        else {
//            // not enough space in RAM
//            printf("Error: Not enough RAM to add program\n");
//            * end = -1;
//            unloadRAM(* start, RAM_SIZE - 1);
//            return;
//        }
//    }
//    * end = idx - 1;
//}


void addLineToRAM(char * line, int index) {
    if(index < RAM_SIZE) {
        ram[index] = strdup(line);
    }
    else {
        // not enough space in RAM
        printf("Error: Not enough RAM to add Line\n");
    }
}


int findFreeFrame(void) {
    for(int i=0; i<RAM_SIZE-3; i+=4) {
        if(ram[i] == NULL) return i / 4;
    }
    return -1;
}

 
void unloadRAM(int pageTable[10]) {
    for(int i = 0; i < 10; i++) {
        if(pageTable[i] != -1) {
            ram[pageTable[i] * 4] = NULL;
            ram[(pageTable[i] * 4) + 1] = NULL;
            ram[(pageTable[i] * 4) + 2] = NULL;
            ram[(pageTable[i] * 4) + 3] = NULL;
        }
    }
}

void unloadFrame(int frame) {
    if(frame >= 0 && frame < 10) {
        ram[frame * 4] = NULL;
        ram[(frame * 4) + 1] = NULL;
        ram[(frame * 4) + 2] = NULL;
        ram[(frame * 4) + 3] = NULL;
    }
}

char * getRAMCell(int i) {
    return ram[i];
}
