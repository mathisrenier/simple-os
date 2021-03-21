//
//  memorymanager.c
//  simple_os
//
//  Created by Mathis Renier on 2021-03-15.
//

#include "memorymanager.h"


// - - - - - - - - - - PRIVATE - - - - - - - - - -

int countTotalPages(FILE * f) {
    int lines = 0;
    
    char c = fgetc(f);
    while(c != EOF) {
        if(c == '\n')
            lines ++;
        c = fgetc(f);
    }
    rewind(f);
    
    return ceil((double)lines / FRAME_SIZE);
}

// - - - - - - - - - - - - - - - - - - - - - - - -

void loadPage(int pageNumber, FILE * f, int frameNumber) {
    if(pageNumber <= countTotalPages(f) && frameNumber <= RAM_SIZE / FRAME_SIZE) {
        
        // skip to right page
        char line[100];
        for(int i=0; i < pageNumber * FRAME_SIZE; i++) {
            fgets(line, 100, f);
        }
        
        for(int i=0; i < FRAME_SIZE; i++) {
            if(fgets(line, 100, f) != NULL) {
                addLineToRAM(line, (frameNumber * FRAME_SIZE) + i);
            }
        }
    }
    rewind(f);
}


int findFrame(void) {
    return findFreeFrame();
}


//int findVictim(PCB * p, ReadyQueueNode * node) {
//    srand((unsigned int) time(NULL));  
//    int frameNum = floor((rand() % RAM_SIZE) / (double)4);
//    
//    do {
//        int * pageTable = getPageTable(getPCB(node));
//        
//        while(isInPageTable(p, frameNum) || (pageTable[getPC_page(getPCB(node))] == frameNum)) {
//            frameNum = (frameNum + 1) % 10;
//        }
//    
//        node = getNextNode(node);
//        
//    } while(node);
//    
//    
//    return frameNum;
//}


int updatePageTable(PCB * p, int pageNumber, int frameNumber, int victimFrame) {
    // victim frame is a dummy variavble
    if(victimFrame) {
        updatePCBPageTable(p, pageNumber, -1);
    }
    else {
        updatePCBPageTable(p, pageNumber, frameNumber);
    }
    return 0;
}

int laucher(FILE * p, char * fileName) {
    // copy file into the Backing Store
    char path[200] = "BackingStore/";
    strcat(path, fileName);
    FILE * newFile = fopen(path, "w+");
    
    // duplicate file
    char c = fgetc(p);
    while(c != EOF) {
        fputc(c, newFile);
        c = fgetc(p);
    }
    
    // close file pointer pointing to the original file
    fclose(p);
    rewind(newFile);
    
    // load at most 2 pages in RAM
    int totalPages = countTotalPages(newFile);
    PCB * pcb;
    
    if(totalPages < 1) {
        printf("Error: No page foud in %s\n", fileName);
        return 0;
    }
    else if(totalPages == 1) {
        // only max 6 frames occupied at launch so no need to call findVictim
        int frameNumber = findFrame();
        loadPage(0, newFile, frameNumber);
        
        pcb = makePCB(frameNumber, totalPages, 0, newFile);
        updatePageTable(pcb, 0, frameNumber, 0);
    }
    else {
        int frameNumber0 = findFrame();
        loadPage(0, newFile, frameNumber0);
        
        int frameNumber1 = findFrame();
        loadPage(1, newFile, frameNumber1);
        
        pcb = makePCB(frameNumber0, totalPages, 1, newFile);
        updatePageTable(pcb, 0, frameNumber0, 0);
        updatePageTable(pcb, 1, frameNumber1, 0);
    }
    
    addToReady(pcb); 
    
    return 1;
}
