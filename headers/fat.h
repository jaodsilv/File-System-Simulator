#ifndef FAT_H
#define FAT_H
#include <stdbool.h>
#include "bitmap.h"
#include "fildir.h"

/*File Allocation Table (fat) size is the total number of blocks*/
#define FAT_SIZE BITMAP_SIZE / CLUSTER_SIZE
/*Used by the fat to determine block number*/
#define BLOCK(bitmap_index) bitmap_index / CLUSTER_SIZE
/*States*/
#define END_OF_FILE -1
#define NOT_IN_USE  -2

/*Used to allocate the files in the file system. From the files we can have the first block position to the last*/
int fat[FAT_SIZE];

void fat_init();
void fat_use(int, unsigned int, bool);
void fat_release(int);

#endif
