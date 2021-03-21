//
//  cpu.h
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#ifndef cpu_h
#define cpu_h

#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"
#include "ram.h"
#include "interpreter.h"

typedef struct CPU CPU;

CPU * instantiateCPU(int quanta);
void resetCPU(int quanta);
CPU * getCPU(void);
int getIP(void);
void setIP(int IP);
int getOffset(void);
void setOffset(int offset);
int isCPUAvailable(void);
int run(int quanta);

#endif /* cpu_h */
