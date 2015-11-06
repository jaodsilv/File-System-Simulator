#ifndef BITMAP_H
#define BITMAP_H
#include <stdint.h>

/*Maximum size in bytes supported by the file system simulator is 100 MB = 100000000 bytes*/
#define BITMAP_SIZE  100000000
/*Used by the bitmap to determine sector state*/
#define ALLOCATED    1
#define UNALLOCATED -1

/*Bitmap to do the memory management. Each index represent 1 byte of the File System*/
int8_t bitmap[BITMAP_SIZE];

/*Prototypes*/
void init_bitmap();

#endif
