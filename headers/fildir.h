#ifndef FILDIR_H
#define FILDIR_H
#include <time.h>
#include <stdio.h>
#include <stdint.h>

/*Size of DATE_FORMAT*/
#define DATE_FORMAT_SIZE 20
/*Name maximum size*/
#define FNAME_SIZE       1024

/*A file structure. Files must be inside directories and will be stored in a doubly linked list*/
typedef struct file {
  /*Metadata fields*/
  /*Absolute path to a file is path + name*/
  char name[FNAME_SIZE];            /*File name. NOTE: file name will NEVER end with '/'*/
  /*Creation, Last Modification and Last Access dates*/
  char creation[DATE_FORMAT_SIZE];
  char modification[DATE_FORMAT_SIZE];
  char access[DATE_FORMAT_SIZE];
  /*Data + Metadata size*/
  unsigned int size;                /*File total size. This is, Data + Metadata*/
  /*FAT first cluster*/
  uint16_t fat_index;               /*First cluster index in file allocation table*/
  /*File list management*/
  struct file *next;                /*Pointer to the next file. If tail, next = NULL*/
  struct file *prev;                /*Pointer to the previous file. If head, prev = NULL*/
} File;

/*A directory sctructure. Each directory is a node in a tree, with ROOT in the simulation being root in the real system*/
typedef struct directory {
  char name[FNAME_SIZE];            /*Directory name. NOTE: directory name will ALWAYS end with '/'*/
  /*Creation, Last Modification and Last Access dates*/
  char creation[DATE_FORMAT_SIZE];
  char modification[DATE_FORMAT_SIZE];
  char access[DATE_FORMAT_SIZE];
  /*FAT first cluster*/
  uint16_t fat_index;               /*First cluster index in file allocation table*/
  /*Files*/
  File *f;                          /*"Head" file of the file lists. If directory has no files, f = NULL. This list contains the files in this directory*/
  /*Directories*/
  struct directory *parent;         /*Parent of this directory. If root, then parent = NULL*/
  struct directory *d;              /*"Head" child directory. If directory has no subdirectories, d = NULL. This list contains the subdirectories of this directory*/
  struct directory *next;           /*Pointer to the next child directory. If tail, next = NULL*/
  struct directory *prev;           /*Pointer to the previous child directory. If head, prev = NULL*/
} Directory;

void get_and_format_time(char *);

#endif
