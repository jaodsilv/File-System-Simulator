/*The root directory is converted to "/" in the simulation. To the program knows who
is the root, the value is stored in the variable named "root" in ep3.c*/
#define ROOT  "/"

/*A file structure. Files must be inside directories and will be stored in a doubly linked list*/
typedef struct file {
  char *name;                       /*File name*/
  unsigned int size;                /*File size*/
  char *created;                    /*Creation date*/
  char *modified;                   /*Last modification date*/
  char *accessed;                   /*Last access date*/
  /*File list management*/
  struct file *next;                /*Pointer to the next file. If tail, next = NULL*/
  struct file *prev;                /*Pointer to the previous file. If head, prev = NULL*/
} File;

/*A directory sctructure. Each directory is a node in a tree, with ROOT in the simulation being root in the real system*/
typedef struct directory {
  char *name;                       /*Directory name*/
  char *created;                    /*Creation date*/
  char *modified;                   /*Last modification date*/
  char *accessed;                   /*Last access date*/
  struct directory *parent;         /*Parent of this directory. If root, then parent = NULL*/
  /*File list*/
  File *f;                          /*"Head" file. If directory is empty, f = NULL*/
  unsigned int files;               /*Total number of files in the list (and thus, in this directory)*/
  struct directory *children_dir;   /*Array with all directories that are direct children of this directory.*/
  unsigned int children;            /*Number of children directories (children_dir size). mkdir calls must start setting this value to zero*/
} Directory;
