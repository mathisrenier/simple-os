//
//  cpu.c
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#include "cpu.h"

// - - - - - - - - - - PRIVATE - - - - - - - - - -

struct CPU {
    int IP;
    char IR[1000];
    int quanta;
    int offset;
};

CPU * cpu;

// - - - - - - - - - - - - - - - - - - - - - - - -

CPU * instantiateCPU(int quanta) {
    cpu = (CPU *) malloc(sizeof(CPU));
    cpu->IP = -1;
    cpu->quanta = quanta;
    cpu->offset = 0;
    
    return cpu;
}

void resetCPU(int quanta) {
    cpu->IP = -1;
    cpu->quanta = quanta;
}

int getIP(void) {
    return cpu->IP;
}

void setIP(int IP) {
    cpu->IP = IP;
}

int getOffset(void) {
    return cpu->offset;
}

void setOffset(int offset) {
    cpu->offset = offset;
}

int isCPUAvailable(void) {
    if(!cpu->quanta || cpu->IP == -1) return 1;
    return 0;
}

int run(int quanta) {
    
    for(int i = 0; i < quanta; i++) {
        char * ramCell = getRAMCell((cpu->IP * 4) + cpu->offset);
        if(ramCell) {
            strcpy(cpu->IR, ramCell);
            
            char * args[MAX_ARGS_NUM];
            clean(args);
            parse(cpu->IR, args);
            interpreter(args);
            clean(args);
        }
            
        cpu->offset++;
        
        if(cpu->offset == 4) {
            cpu->quanta = 0;
            return 1;
        }
    }
    
    cpu->quanta = 0;
    return 0;
}
