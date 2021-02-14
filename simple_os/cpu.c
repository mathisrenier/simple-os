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
};

CPU * cpu;

// - - - - - - - - - - - - - - - - - - - - - - - -

CPU * instantiateCPU(int quanta) {
    cpu = (CPU *) malloc(sizeof(CPU));
    cpu->IP = -1;
    cpu->quanta = quanta;
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

int isCPUAvailable(void) {
    if(!cpu->quanta || cpu->IP == -1) return 1;
    return 0;
}

void run(int quanta) {
    
    for(int i = 0; i < quanta; i++) {
        strcpy(cpu->IR, getRAMCell(cpu->IP));
        
        char * args[MAX_ARGS_NUM];
        clean(args);
        parse(cpu->IR, args);
        interpreter(args);
        clean(args);
        
        cpu->IP++;
    }
    
    cpu->quanta = 0;
}
