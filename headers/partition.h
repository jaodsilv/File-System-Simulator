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
#define FRESH_FS       24968                               /*Number of free blocks in a file system that contains only the root directory*/

/*Binary Layout Positions*/
#define SUPERBLOCK       0                                 /*Superblock contains the number of free blocks.*/
#define FREE_SPACE_BLOCK SUPERBLOCK + 2                    /*The bitmap array*/
#define FAT_BLOCK        FREE_SPACE_BLOCK + TOTAL_BLOCKS   /*The fat table*/
#define ROOT_DIR_NAME    FAT_BLOCK + (TOTAL_BLOCKS * 2)    /*Root directory name*/
#define ROOT_DIR_CREAT   ROOT_DIR_NAME + 2                 /*Root directory creation date*/
#define ROOT_DIR_MODIF   ROOT_DIR_CREAT + DATE_FORMAT_SIZE /*Root directory modification date*/
#define ROOT_DIR_ACCESS  ROOT_DIR_MODIF + DATE_FORMAT_SIZE /*Root directory access date*/

#define FILES_AND_SUBDIR     76000                         /*Starting position for files and directories after root*/
#define FD_FIRST_BLOCK       FILES_AND_SUBDIR / BLOCK_SIZE /*Block number*/
/*Files and Directories*/
#define FD_BITMAP_INDEX      0                             /*Index in the bitmap of this block*/
#define FD_FAT_INDEX         FD_BITMAP_INDEX + 2           /*Index in the FAT table*/
#define FD_FAT_CONTENT       FD_FAT_INDEX + 2              /*Context of FAT in the given index of this block*/
#define FD_FAT_FIRST_INDEX   FD_FAT_CONTENT + 2            /*Initial index for the chain of blocks of this file*/
#define FILE_SIZE            FD_FAT_FIRST_INDEX + 2        /*Size of the file*/
#define FD_NAME              FILE_SIZE + 8                 /*Absolute name of the file or directory. Files must have at most 1024 characters*/
#define FD_CDATE             FD_NAME + FNAME_SIZE          /*File/Directory creation date*/
#define FD_MDATE             FD_CDATE + DATE_FORMAT_SIZE   /*File/Directory modified date*/
#define FD_ADATE             FD_MDATE + DATE_FORMAT_SIZE   /*File/Directory access date*/
#define FILE_DATA            FD_ADATE + DATE_FORMAT_SIZE   /*File contents. If doesn't fit, must use another block. Consumes remaining 2884 bytes*/
#define NEXT_BLOCK           4000                          /*Add this to the pointer location in a fseek call, multiplied by i [0, last block]*/

/*Metadata consumes the first 1100 bytes of the block, leaving the remaining 2900 bytes to data*/
#define DATA_LIMIT           2900

#endif
