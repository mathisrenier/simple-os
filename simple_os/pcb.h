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
void getStartEnd(PCB * pcb, int * start, int * end);
int isExecDone(PCB * pcb);
PCB * makePCB(int start, int end);

#endif /* pcb_h */
