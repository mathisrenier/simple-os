//
//  DISK_driver.c
//  simple_os
//
//  Created by Mathis Renier on 2021-04-05.
//

#include "DISK_driver.h"


// - - - - - - - - - - PRIVATE - - - - - - - - - - - - - - -

struct PARTITION {
    int total_blocks;
    int block_sizes;
} PARTITION;


struct FAT {
    char filename[50];
    int file_length;
    int current_location;
    int blockPtrs[10];
} fat[20];


char * block_buffer;
FILE * active_file_table[5];
int fatToAft[20];  // tracks which active file table belongs to which fat
FILE * active_partition;
int current_block;


void initPartition(FILE * f) {
    // fat
    for(int i=0; i<20; i++) {
        strcpy(fat[i].filename, "NULL");
        fat[i].file_length = 0;
        fat[i].current_location = -1;
        
        for(int j=0; j<10; j++) {
            fat[i].blockPtrs[j] = -1;
        }
    }
    closeAllFiles();
    
    // active file table
    for(int i=0; i<5; i++) {
        active_file_table[i] = NULL;
    }
    
    // fat to aft table
    for(int i=0; i<20; i++) {
        fatToAft[i] = -1;
    }
    
    current_block = 0;
    
    rewind(f);
    
    // partition section
    fprintf(f, "%d %d \n", PARTITION.total_blocks, PARTITION.block_sizes);
    
    // fat section
    for(int i=0; i<20; i++) {
        fprintf(f, "%s %d %d ", fat[i].filename, fat[i].file_length, fat[i].current_location);
        
        for(int j=0; j<10; j++) {
            fprintf(f, "%d ", fat[i].blockPtrs[j]);
        }
        fprintf(f, "\n");
    }
    
    // data section
    int totalDataCells = PARTITION.total_blocks * PARTITION.block_sizes;
    for(int i=0; i<totalDataCells; i++) {
        fprintf(f, "0 ");
    }
    
    rewind(f);
}


/// returns the index of the FAT containing the name or -1 otherwise
int findName(char * name) {
    for(int i=0; i<20; i++) {
        if(!strcmp(fat[i].filename, name)) return i;
    }
    return -1;
}


int findFreeAftCell(void) {
    for(int i=0; i<5; i++) {
        if(!active_file_table[i]) return i;
    }
    return -1;
}


void seekDataSection(FILE * f) {
    rewind(f);
    
    // partition section
    fscanf(f, "%*d %*d \n");
    
    // fat section
    for(int i=0; i<20; i++) {
        fscanf(f, "%*s %*d %*d ");
        
        for(int j=0; j<10; j++) {
            fscanf(f, "%*d ");
        }
//        fgetc(f);
    }
}


void seekBlock(FILE * f, int fatIdx, int blockPtrIdx) {
    int block = fat[fatIdx].blockPtrs[blockPtrIdx];
    
    seekDataSection(f);
    
    for(int i=0; i < block * PARTITION.block_sizes * 2; i++) {
        fgetc(f);
    }
}


void updateDisk(void) {
    // store data
    char data[5000];
    FILE * f = fdopen(dup(fileno(active_partition)), "r+");
    seekDataSection(f);
    fgets(data, 5000, f);
    
    // erase file
    f = freopen(NULL, "w+", f);
    
    // rewrite data
    // partition section
    fprintf(f, "%d %d \n", PARTITION.total_blocks, PARTITION.block_sizes);
    
    // fat section
    for(int i=0; i<20; i++) {
        fprintf(f, "%s %d %d ", fat[i].filename, fat[i].file_length, fat[i].current_location);
        
        for(int j=0; j<10; j++) {
            fprintf(f, "%d ", fat[i].blockPtrs[j]);
        }
        fprintf(f, "\n");
    }
    
    // data section
    fputs(data, f);
    
    fclose(f);
}


void updateDiskNewData(char * data) {
    // duplicate ptr and erase file
    FILE * f = fdopen(dup(fileno(active_partition)), "r+");
    f = freopen(NULL, "w+", f);
    
    // rewrite data
    // partition section
    fprintf(f, "%d %d \n", PARTITION.total_blocks, PARTITION.block_sizes);
    
    // fat section
    for(int i=0; i<20; i++) {
        fprintf(f, "%s %d %d ", fat[i].filename, fat[i].file_length, fat[i].current_location);
        
        for(int j=0; j<10; j++) {
            fprintf(f, "%d ", fat[i].blockPtrs[j]);
        }
        fprintf(f, "\n");
    }
    
    // data section
    fputs(data, f);
    
    fclose(f);
}


int isEOF(int fatIdx) {
    if(fat[fatIdx].current_location >= fat[fatIdx].file_length)
        return 1;
    
    return 0;
}


void loadBlockBuffer(FILE * f, int fatIdx) {
    seekBlock(f, fatIdx, fat[fatIdx].current_location);
    int j = 0;
    for(int i=0; i<PARTITION.block_sizes * 2; i++) {
        char c = fgetc(f);
        
        if(c == '0') {
            block_buffer[j] = '\0';
            return;
        }
        
        if(c != ' ') {
            block_buffer[j] = c;
            j++;
        }
    }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void initIO(void) {
    // Partition
    PARTITION.total_blocks = 0;
    PARTITION.block_sizes = 0;
    
    // fat
    for(int i=0; i<20; i++) {
        strcpy(fat[i].filename, "NULL");
        fat[i].file_length = 0;
        fat[i].current_location = -1;
        
        for(int j=0; j<10; j++) {
            fat[i].blockPtrs[j] = -1;
        }
    }
    
    // active file table
    for(int i=0; i<5; i++) {
        active_file_table[i] = NULL;
    }
    
    // fat to aft table
    for(int i=0; i<20; i++) {
        fatToAft[i] = -1;
    }
    
    current_block = 0;
}


int partition(char * name, int blocksize, int totalblocks) {
    system("if [ ! -d PARTITION ]; then mkdir PARTITION; fi");
    
    char path[200] = "PARTITION/";
    strcat(path, name);
    FILE * file = fopen(path, "r+");
    
    // if file does not exist
    if(!file) {
        fclose(file);
        file = fopen(path, "a+");
        PARTITION.total_blocks = totalblocks;
        PARTITION.block_sizes = blocksize;
        
        initPartition(file);
        
        fclose(file);
    }
    
    
    
    return 1;
}


int mountFS(char * name) {
    char path[200] = "PARTITION/";
    strcat(path, name);
    FILE * file = fopen(path, "r+");
    
    // partition section
    fscanf(file, "%d %d \n", &PARTITION.total_blocks, &PARTITION.block_sizes);
    
    // fat section
    for(int i=0; i<20; i++) {
        fscanf(file, "%s %d %d ", fat[i].filename, &fat[i].file_length, &fat[i].current_location);
        
        for(int j=0; j<10; j++) {
            fscanf(file, "%d ", &fat[i].blockPtrs[j]);
        }
//        fgetc(file);
    }
    
    rewind(file);
    
    block_buffer = (char *) malloc(PARTITION.block_sizes * sizeof(char));
    active_partition = file;
    
    return 1;
}


int openfile(char * name) {
    int fatIdx = findName(name);
    
    // name not found
    if(fatIdx == -1) {
        // find free fat cell
        fatIdx = findName("NULL");
        if(fatIdx != -1) {
            if(current_block <= PARTITION.total_blocks - 10) {
                // init new file
                strcpy(fat[fatIdx].filename, name);
                fat[fatIdx].current_location = 0;
                
                for(int i=0; i<10; i++) {
                    fat[fatIdx].blockPtrs[i] = current_block + i;
                }
                current_block += 10;
                
                updateDisk();
                
                int aftIdx = findFreeAftCell();
                if(aftIdx != -1) {
                    fatToAft[fatIdx] = aftIdx;
                    active_file_table[aftIdx] = fdopen(dup(fileno(active_partition)), "r+");
                    
                    seekBlock(active_file_table[aftIdx], fatIdx, 0);
                    return fatIdx;
                }
                
            }
        }
    }
    // name found
    else {
        fat[fatIdx].current_location = 0;
        
        int aftIdx = findFreeAftCell();
        if(aftIdx != -1) {
            fatToAft[fatIdx] = aftIdx;
            active_file_table[aftIdx] = fdopen(dup(fileno(active_partition)), "r+");
            
            seekBlock(active_file_table[aftIdx], fatIdx, 0);
            return fatIdx;
        }
    }
    
    return -1;
}


void closefile(int fatIdx) {
    int aftIdx = fatToAft[fatIdx];
    fclose(active_file_table[aftIdx]);
    active_file_table[aftIdx] = NULL;
    fatToAft[fatIdx] = -1;
}


char * readBlock(int file) {
    if(file >= 0 && file < 20) {
        FILE * f = active_file_table[fatToAft[file]];
        
        if(!isEOF(file)) {
            loadBlockBuffer(f, file);
            fat[file].current_location += 1;
            updateDisk();
            
            return block_buffer;
        }
    }
    
    
    return NULL;
}


int writeBlock(int file, char * data) {
    if(file >= 0 && file < 20) {
        FILE * f = active_file_table[fatToAft[file]];
        
        char partitionData[5000];
        seekDataSection(f);
        fgets(partitionData, 5000, f);
        
        if(fat[file].current_location < 10) {
            // modify data
            int block = fat[file].blockPtrs[fat[file].current_location];
            int index = block * PARTITION.block_sizes * 2;  // index of the block in the data array. *2 because of the space
            
            int j = 0;
            for(int i=index; i < index + PARTITION.block_sizes * 2; i++) {
                if(j < strlen(data) && partitionData[i] != ' ') {
                    partitionData[i] = data[j];
                    j++;
                }
                
            }
            
            // modify fat
            if(fat[file].current_location >= fat[file].file_length) fat[file].file_length += 1;
            fat[file].current_location += 1;
            
            updateDiskNewData(partitionData);
            
            return 0;
        }
    }
    
    return 1;
}


void closeAllFiles(void) {
    for(int i=0; i< 5; i++) {
        if(active_file_table[i]) fclose(active_file_table[i]);
    }
    fclose(active_partition);
}


int getBlockSize(void) {
    return PARTITION.block_sizes;
}


void freeBlockBuffer(void) {
    free(block_buffer);
}


