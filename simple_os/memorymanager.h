//
//  memorymanager.h
//  simple_os
//
//  Created by Mathis Renier on 2021-03-15.
//

#ifndef memorymanager_h
#define memorymanager_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "pcb.h"
#include "ram.h"
#include "kernel.h"

#define FRAME_SIZE 4

void loadPage(int pageNumber, FILE * f, int frameNumber);
int findFrame(void);
int updatePageTable(PCB * p, int pageNumber, int frameNumber, int victimFrame);
int laucher(FILE * p, char * fileName);


#endif /* memorymanager_h */
