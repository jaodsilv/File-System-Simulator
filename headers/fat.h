#ifndef FAT_H
#define FAT_H
#include "bitmap.h"

/*Blocks of size 4kb = 4000bytes*/
#define CLUSTER_SIZE 4000
/*File Allocation Table (fat) size is the total number of blocks*/
#define FAT_SIZE BITMAP_SIZE / CLUSTER_SIZE
/*Used by the fat to determine block number*/
#define BLOCK(bitmap_index) bitmap_index / CLUSTER_SIZE
/*States*/
#define END_OF_FILE -1
#define IN_USE(index) index >= 0
#define NOT_IN_USE  -2

/*Used to allocate the files in the file system. From the files we can have the first block position to the last*/
int fat[FAT_SIZE];

void init_fat();
void fat_use(int, unsigned int);
void fat_release(int);

#endif
