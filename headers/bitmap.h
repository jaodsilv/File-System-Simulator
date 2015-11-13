#ifndef BITMAP_H
#define BITMAP_H
#include <stdint.h>
#include <stdbool.h>
#include "fildir.h"
#include "partition.h"

/*Maximum size in bytes supported by the file system simulator is 100 MB = 100000000 bytes*/
#define BITMAP_SIZE  PARTITION_SIZE / BLOCK_SIZE
/*Used by the bitmap to determine sector state*/
#define ALLOCATED    1
#define UNALLOCATED  0

/*Bitmap to do the memory management. Each index represent 1 byte of the File System*/
uint8_t bitmap[BITMAP_SIZE];

/*Prototypes*/
void bitmap_init_new();

#endif
