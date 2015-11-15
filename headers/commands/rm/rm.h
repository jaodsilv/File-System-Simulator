#ifndef RM_H
#define RM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../fat.h"
#include "../../fildir.h"
#include "../../partition.h"

#define SUCCESS     1
#define BAD_PATH_RM 2

int cmd_rm(char *, int, char **, char *, Directory *, bool);
int remove_file(char *, char *, Directory *);

#endif
