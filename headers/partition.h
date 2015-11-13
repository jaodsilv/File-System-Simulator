#ifndef PARTITION_H
#define PARTITION_H
#include <stdint.h>
#include <stdbool.h>
#include "fildir.h"

/*The root directory is converted to "/" in the simulation. To the program knows who
is the root, the value is stored in the variable named "root" in ep3.c*/
#define ROOT           "/"
/*Partition size is 100mb*/
#define PARTITION_SIZE 100000000
/*Blocks of size 4kb = 4000bytes*/
#define BLOCK_SIZE     4000
/*Total number of blocks*/
#define TOTAL_BLOCKS   PARTITION_SIZE / BLOCK_SIZE

/*Binary Layout Positions*/
#define SUPERBLOCK       0                                 /*Superblock contains the number of free blocks.*/
#define FREE_SPACE_BLOCK SUPERBLOCK + 8                    /*The bitmap array*/
#define FAT_BLOCK        FREE_SPACE_BLOCK + TOTAL_BLOCKS   /*The fat table*/
#define ROOT_DIR_NAME    FAT_BLOCK + (TOTAL_BLOCKS * 2)    /*Root directory name*/
#define ROOT_DIR_CREAT   ROOT_DIR_NAME + 2                 /*Root directory creation date*/
#define ROOT_DIR_MODIF   ROOT_DIR_CREAT + DATE_FORMAT_SIZE /*Root directory modification date*/
#define ROOT_DIR_ACCESS  ROOT_DIR_MODIF + DATE_FORMAT_SIZE /*Root directory access date*/
#define FILES_AND_SUBDIR 128000

#define FD_FIRST_BLOCK   FILES_AND_SUBDIR / BLOCK_SIZE

#endif
