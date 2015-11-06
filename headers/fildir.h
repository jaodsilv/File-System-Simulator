#ifndef FILDIR_H
#define FILDIR_H

/*The root directory is converted to "/" in the simulation. To the program knows who
is the root, the value is stored in the variable named "root" in ep3.c*/
#define ROOT             '/'
/*String format to creation, access and modification of files and directories*/
#define DATE_FORMAT      "%d/%m/%Y %H:%M:%S"
/*Size of DATE_FORMAT*/
#define DATE_FORMAT_SIZE 30

/*A file structure. Files must be inside directories and will be stored in a doubly linked list*/
typedef struct file {
  /*Metadata fields*/
  char *name;                       /*File name*/
  char created[DATE_FORMAT_SIZE];   /*Creation date*/
  char modified[DATE_FORMAT_SIZE];  /*Last modification date*/
  char accessed[DATE_FORMAT_SIZE];  /*Last access date*/
  /*Data fields*/
  unsigned int size;                /*File contents size*/
  /*Data + Metadata size*/
  unsigned int total_size;          /*File total size*/
  /*Memory position (bitmap_index)*/
  unsigned int bitmap_index;        /*Starting index in the bitmap array. Allocated in range [bitmap_index, bitmap_index + total_size - 1]*/
  /*FAT first cluster*/
  int fat_index;                    /*First cluster index in file allocation table*/
  /*File list management*/
  struct file *next;                /*Pointer to the next file. If tail, next = NULL*/
  struct file *prev;                /*Pointer to the previous file. If head, prev = NULL*/
} File;

/*A directory sctructure. Each directory is a node in a tree, with ROOT in the simulation being root in the real system*/
typedef struct directory {
  char *name;                       /*Directory name*/
  char created[DATE_FORMAT_SIZE];   /*Creation date*/
  char modified[DATE_FORMAT_SIZE];  /*Last modification date*/
  char accessed[DATE_FORMAT_SIZE];  /*Last access date*/
  /*Files*/
  File *f;                          /*"Head" file. If directory is empty, f = NULL*/
  unsigned int files;               /*Total number of files in the list (and thus, in this directory)*/
  /*Directories*/
  struct directory *parent;         /*Parent of this directory. If root, then parent = NULL*/
  struct directory *d;              /*"Head" child directory*/
  struct directory *next;           /*Pointer to the next child directory. If tail, next = NULL*/
  struct directory *prev;           /*Pointer to the previous child directory. If head, prev = NULL*/
  unsigned int children;            /*Number of directories in this directory (= children directories)*/
} Directory;

#endif
