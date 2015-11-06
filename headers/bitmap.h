#ifndef BITMAP_H
#define BITMAP_H
#include <stdint.h>
#include <stdbool.h>
#include "fildir.h"

/*Maximum size in bytes supported by the file system simulator is 100 MB = 100000000 bytes*/
#define BITMAP_SIZE  100000000
/*Blocks of size 4kb = 4000bytes*/
#define CLUSTER_SIZE 4000
/*Used by the bitmap to determine sector state*/
#define ALLOCATED    1
#define UNALLOCATED -1

/*Bitmap to do the memory management. Each index represent 1 byte of the File System*/
int8_t bitmap[BITMAP_SIZE];

/*Prototypes*/
void bitmap_init();
unsigned int total_free_space();
unsigned int total_wasted_space();
unsigned int required_clusters(unsigned int);
unsigned int get_starting_index(unsigned int);
void allocate_file(unsigned int, unsigned int);
bool file_allocated(File *);
void free_file(unsigned int, unsigned int);
bool file_freed(File *);

#endif
