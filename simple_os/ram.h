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


void resetRam(void);
void addToRAM(FILE * p, int * start, int * end);
void unloadRAM(int start, int end);
char * getRAMCell(int i);


#endif /* ram_h */
