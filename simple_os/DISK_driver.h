//
//  DISK_driver.h
//  simple_os
//
//  Created by Mathis Renier on 2021-04-05.
//

#ifndef DISK_driver_h
#define DISK_driver_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void initIO(void);
int partition(char * name, int blocksize, int totalblocks);
int mountFS(char * name);
int openfile(char * name);
void closefile(int fatIdx);
char * readBlock(int file);
int writeBlock(int file, char * data);
void closeAllFiles(void);
int getBlockSize(void);


#endif /* DISK_driver_h */
