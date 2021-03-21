//
//  ram.h
//  simple_os
//
//  Created by Mathis Renier on 2021-02-10.
//

#ifndef ram_h
#define ram_h

#include <stdio.h>
#include <string.h>

#define RAM_SIZE 40

void resetRam(void);
void addToRAM(FILE * p, int * start, int * end);
void addLineToRAM(char * line, int index);
int findFreeFrame(void);
void unloadRAM(int pageTable[10]);
void unloadFrame(int frame);
char * getRAMCell(int i);


#endif /* ram_h */
