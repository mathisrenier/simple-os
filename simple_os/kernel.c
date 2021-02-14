//
//  kernel.c
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#include "kernel.h"

// - - - - - - - - - - PRIVATE - - - - - - - - - -

struct ReadyQueueNode {
    PCB * pcb;
    ReadyQueueNode * next;
    
};

struct ReadyQueue {
    ReadyQueueNode * head;
    ReadyQueueNode * tail;
};

ReadyQueue * readyQueue;


PCB * dequeueReady(void) {
    ReadyQueueNode * tmp;
    
    PCB * pcb = readyQueue->head->pcb;
    tmp = readyQueue->head;
    readyQueue->head = readyQueue->head->next;
    
    if(readyQueue->tail == tmp) {
        readyQueue->tail = NULL;
    }
    
    free(tmp);
    
    return pcb;
}

void bigPurge(void) {
    ReadyQueueNode * tmp;
    
    while(readyQueue->head) {
        if(readyQueue->head->pcb) free(readyQueue->head->pcb);
        tmp = readyQueue->head;
        readyQueue->head = readyQueue->head->next;
        free(tmp);
    }
    
    readyQueue->head = NULL;
    readyQueue->tail = NULL;
    
    resetRam();
    resetCPU(QUANTA);
}

// returns the number of lines left to execute in the pcb
int taskSwitchIn(void) {
    setIP(getPC(readyQueue->head->pcb));
    
    int start;
    int end;
    getStartEnd(readyQueue->head->pcb, &start, &end);
    return end - getPC(readyQueue->head->pcb) + 1;
}

void taskSwitchOut(void) {
    PCB * pcb = dequeueReady();
    updatePC(pcb, getIP());
    
    if(isExecDone(pcb)) {
        int start;
        int end;
        getStartEnd(pcb, &start, &end);
        unloadRAM(start, end);
        free(pcb);
    }
    else {
        addToReady(pcb);
    }
}


// - - - - - - - - - - - - - - - - - - - - - - - -


void addToReady(PCB * pcb) {
    ReadyQueueNode * newNode = (ReadyQueueNode *) malloc(sizeof(ReadyQueueNode));
    newNode->pcb = pcb;
    newNode->next = NULL;
    
    if(readyQueue->tail) {
        readyQueue->tail->next = newNode;
        readyQueue->tail = readyQueue->tail->next;
    }
    else {
        readyQueue->tail = newNode;
        readyQueue->head = newNode;
    }
}


int myinit(char * filename) {
    FILE * file = fopen(filename, "r");
    
    if(!file) {
        printf("Error: Script %s could not be loaded\n", filename);
        bigPurge();
        return 1;
    }
    
    int start;
    int end;
    addToRAM(file, &start, &end);
    
    // RAM full
    if(end == -1) {
        bigPurge();
        return 2;
    }
    
    PCB * pcb = makePCB(start, end);
    updatePC(pcb, start);
    
    addToReady(pcb);
    
    return 0;
}


void scheduler(void) {
    while(readyQueue->head) {
        if(isCPUAvailable()) {
            int lineleft = taskSwitchIn();
            if(lineleft < QUANTA) {
                run(lineleft);
            }
            else {
                run(QUANTA);
            }
            taskSwitchOut();
        }
    }
    bigPurge();
}


int main(int argc, char * argv[]) {
    CPU * cpu = instantiateCPU(QUANTA);
    readyQueue = (ReadyQueue *) malloc(sizeof(ReadyQueue));
    readyQueue->head = NULL;
    readyQueue->tail = NULL;
    
    
    printf("Kernel 1.0 loaded!\n");
    shellUI();
    
    free(cpu);
}
