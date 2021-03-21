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
        if(readyQueue->head->pcb) {
            fclose(getFilePtr(readyQueue->head->pcb));
            free(readyQueue->head->pcb);
        }
        
        tmp = readyQueue->head;
        readyQueue->head = readyQueue->head->next;
        free(tmp);
    }
    
    readyQueue->head = NULL;
    readyQueue->tail = NULL;
    
    resetRam();
    resetCPU(QUANTA);
}

int findVictim(PCB * p) {
    srand((unsigned int) time(NULL));
    int frameNum = floor((rand() % RAM_SIZE) / (double)4);
    
    ReadyQueueNode * tmp = readyQueue->head;
    do {
        int * pageTable = getPageTable(tmp->pcb);
        
        while(isInPageTable(p, frameNum) || (pageTable[getPC_page(tmp->pcb)] == frameNum)) {
            frameNum = (frameNum + 1) % 10;
        }
    
        tmp = tmp->next;
        
    } while(tmp);
    
    
    return frameNum;
}

void updateVictimPCB(int frame) {
    ReadyQueueNode * tmp = readyQueue->head;
    
    do {
        int * pageTable = getPageTable(tmp->pcb);
        for(int i=0; i<10; i++) {
            if(pageTable[i] == frame) {
                updatePageTable(tmp->pcb, i, -1, 1); 
                return;
            }
        }
        tmp = tmp->next;
        
    } while(tmp);
}

// returns the number of lines left to execute in the page
int taskSwitchIn(void) {
    setIP(getPC(readyQueue->head->pcb));
    
    int offset = getPC_offset(readyQueue->head->pcb);
    setOffset(offset);
    
    return 4 - offset;
}

void taskSwitchOut(void) {
    PCB * pcb = dequeueReady();
    
    updatePC(pcb, getIP());
    updatePC_offset(pcb, getOffset());
    
    if(isExecDone(pcb)) {
        unloadRAM(getPageTable(pcb));
        free(pcb);
    }
    else {
        addToReady(pcb);
    }
}

void pageFaultTaskSwitch(void) {
    PCB * pcb = dequeueReady();
    
    updatePC_page(pcb, getPC_page(pcb) + 1);
    
    if(isExecDone(pcb)) {
        unloadRAM(getPageTable(pcb));
        free(pcb);
        return;
    }
    
    int * pageTable = getPageTable(pcb);
    
    // if next entry in the page table exists
    if(pageTable[getPC_page(pcb)] != -1) {
        updatePC(pcb, pageTable[getPC_page(pcb)]);
        updatePC_offset(pcb, 0);
    }
    else {
        int frame = findFrame();
        
        if(frame == -1) {
            frame = findVictim(pcb);
            updateVictimPCB(frame);
        }
        
        loadPage(getPC_page(pcb), getFilePtr(pcb), frame);
        
        updatePageTable(pcb, getPC_page(pcb), frame, 0);
        updatePC(pcb, frame);
        updatePC_offset(pcb, 0);
    }
    
    updatePageTable(pcb, getPC_page(pcb) - 1, -1, 0);
    unloadFrame(pageTable[getPC_page(pcb) - 1]);
    
    addToReady(pcb);
}

void boot(void) {
    resetRam();
    system("if [ -d BackingStore ]; then rm -rf Backingstore; fi; mkdir BackingStore");
}

int kernel(void) {
    CPU * cpu = instantiateCPU(QUANTA);
    readyQueue = (ReadyQueue *) malloc(sizeof(ReadyQueue));
    readyQueue->head = NULL;
    readyQueue->tail = NULL;
    
    
    printf("Kernel 2.0 loaded!\n");
    shellUI();
    
    free(cpu);
    
    return 0;
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

PCB * getPCB(ReadyQueueNode * node) {
    return node->pcb;
}

ReadyQueueNode * getNextNode(ReadyQueueNode * node) {
    return node->next;
}

//int myinit(char * filename) {
//    FILE * file = fopen(filename, "r");
//
//    if(!file) {
//        printf("Error: Script %s could not be loaded\n", filename);
//        bigPurge();
//        return 1;
//    }
//
//    int start;
//    int end;
//    addToRAM(file, &start, &end);
//
//    // RAM full
//    if(end == -1) {
//        bigPurge();
//        return 2;
//    }
//
//   PCB * pcb = makePCB(start, end);
//    updatePC(pcb, start);
//
//    addToReady(pcb);
//
//    return 0;
//}


void scheduler(void) {
    while(readyQueue->head) {
        if(isCPUAvailable()) {
            int lineleft = taskSwitchIn();
            int pageFault = 0;
            
            if(lineleft < QUANTA) {
                pageFault = run(lineleft);
            }
            else {
                pageFault = run(QUANTA);
            }
            
            if(pageFault) {
                pageFaultTaskSwitch();
            }
            else {
                taskSwitchOut();
            }
            
        }
    }
    bigPurge();
}


int main(int argc, char * argv[]) {
    int error = 0;
    boot();
    error = kernel();
    return error;
}
