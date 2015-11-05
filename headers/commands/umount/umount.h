#ifndef UMOUNT_H
#define UMOUNT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../fildir.h"

int cmd_umount(char *, char *, Directory *, bool);
void reset_root_dir(Directory *);
void kill_children(Directory *);
Directory *get_leaf(Directory *);
void remove_files(Directory *);
void remove_file(File *, Directory *);
void remove_directory(Directory *, Directory *);


#endif
