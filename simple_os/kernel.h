//
//  kernel.h
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#ifndef kernel_h
#define kernel_h

#include <stdio.h>
#include <stdlib.h>
#include "ram.h"
#include "pcb.h"
#include "cpu.h"
#include "shell.h"

#define QUANTA 2

typedef struct ReadyQueueNode ReadyQueueNode;
typedef struct ReadyQueue ReadyQueue;

void addToReady(PCB * pcb);
int myinit(char * filename);
void scheduler(void);

#endif /* kernel_h */