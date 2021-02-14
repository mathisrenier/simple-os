//
//  pcb.c
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#include "pcb.h"

// - - - - - - - - - - PRIVATE - - - - - - - - - -

struct PCB {
    int PC;
    int start;
    int end;
};

// - - - - - - - - - - - - - - - - - - - - - - - -

void updatePC(PCB * pcb, int PC) {
    pcb->PC = PC;
}

int getPC(PCB * pcb) {
    return pcb->PC;
}

void getStartEnd(PCB * pcb, int * start, int * end) {
    * start = pcb->start;
    * end = pcb->end;
}

int isExecDone(PCB * pcb) {
    return pcb->PC > pcb->end;
}

PCB * makePCB(int start, int end) {
    PCB * pcb = (PCB *) malloc(sizeof(PCB));
    pcb->start = start;
    pcb->end = end;
    
    return pcb;
}
