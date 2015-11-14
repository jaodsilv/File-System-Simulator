#ifndef MKDIR_H
#define MKDIR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../fat.h"
#include "../../fildir.h"
#include "../../partition.h"

#define SUCCESS    1
#define NO_MEM     2
#define BAD_PATH   3
#define TOO_LARGE  4
#define NOT_UNIQUE 5

int cmd_mkdir(char *, int, char **, char *, Directory *, bool);
int create_dir(char *, char *, Directory *);

#endif
