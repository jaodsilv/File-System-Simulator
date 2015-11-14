#ifndef FAT_H
#define FAT_H
#include <stdint.h>
#include <stdbool.h>
#include "bitmap.h"
#include "fildir.h"

/*File Allocation Table (fat) size is the total number of blocks*/
#define FAT_SIZE BITMAP_SIZE
/*Used by the fat to determine block number*/
#define BLOCK(bitmap_index) bitmap_index / CLUSTER_SIZE
/*States*/
#define RESERVED    65533
#define AVAILABLE   65534
#define END_OF_FILE 65535

/*Used to allocate the files in the file system. From the files we can have the first block position to the last*/
uint16_t fat[FAT_SIZE];

void fat_init_new();
uint16_t fat_get_index();

#endif
