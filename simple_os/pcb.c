//
//  pcb.c
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#include "pcb.h"

// - - - - - - - - - - PRIVATE - - - - - - - - - -

struct PCB {
//    int start;
//    int end;
    int PC;
    int PC_offset;
    int pages_max;
    int PC_page;
    int pageTable[10];
    FILE * file;
};

void initPageTable(PCB * pcb) {
    for(int i=0; i<10; i++) {
        pcb->pageTable[i] = -1;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - -

void updatePC(PCB * pcb, int PC) {
    pcb->PC = PC;
}

int getPC(PCB * pcb) {
    return pcb->PC;
}

void updatePC_offset(PCB * pcb, int PC_offset) {
    pcb->PC_offset = PC_offset;
}

int getPC_offset(PCB * pcb) {
    return pcb->PC_offset;
}

void updatePC_page(PCB * pcb, int PC_page) {
    pcb->PC_page = PC_page;
}

int getPC_page(PCB * pcb) {
    return pcb->PC_page;
}

//void getStartEnd(PCB * pcb, int * start, int * end) {
//    * start = pcb->start;
//    * end = pcb->end;
//}

int isExecDone(PCB * pcb) {
    return pcb->PC_page >= pcb->pages_max;
}

PCB * makePCB(int PC, int pages_max, int PC_page, FILE * file) {
    PCB * pcb = (PCB *) malloc(sizeof(PCB));
    pcb->PC = PC;
    pcb->PC_offset = 0;
    pcb->pages_max = pages_max;
    pcb->PC_page = PC_page;
    pcb->file = file;
    initPageTable(pcb);
    
    return pcb;
}

int isInPageTable(PCB * pcb, int frame) {
    for(int i=0; i<10; i++) {
        if(pcb->pageTable[i] == frame) {
            return 1;
        }
    }
    return 0;
}

void updatePCBPageTable(PCB * pcb, int page, int frame) {
    pcb->pageTable[page] = frame;    
}

int * getPageTable(PCB * pcb) {
    return pcb->pageTable;
}

FILE * getFilePtr(PCB * pcb) {
    return pcb->file;
}
