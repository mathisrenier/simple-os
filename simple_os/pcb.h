//
//  pcb.h
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#ifndef pcb_h
#define pcb_h

#include <stdio.h>
#include <stdlib.h>

typedef struct PCB PCB;

void updatePC(PCB * pcb, int PC);
int getPC(PCB * pcb);
void updatePC_offset(PCB * pcb, int PC_offset);
int getPC_offset(PCB * pcb);
void updatePC_page(PCB * pcb, int PC_page);
int getPC_page(PCB * pcb);
//void getStartEnd(PCB * pcb, int * start, int * end);
int isExecDone(PCB * pcb);
PCB * makePCB(int PC, int pages_max, int PC_page, FILE * file);
int isInPageTable(PCB * pcb, int frame);
void updatePCBPageTable(PCB * pcb, int page, int frame);
int * getPageTable(PCB * pcb);
FILE * getFilePtr(PCB * pcb);

#endif /* pcb_h */
