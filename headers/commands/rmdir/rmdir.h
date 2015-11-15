#ifndef RMDIR_H
#define RMDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../fat.h"
#include "../../fildir.h"
#include "../../partition.h"

#define SUCCESS        1
#define BAD_PATH_RMDIR 2
#define FORBIDDEN      3

int cmd_rmdir(char *, int, char **, char *, Directory *, bool);
int remove_directory(char *, char *, Directory *);
Directory *get_leaf(Directory *);
void remove_files(char *, Directory *, File *);
void remove_dir(char *, Directory *);

#endif
