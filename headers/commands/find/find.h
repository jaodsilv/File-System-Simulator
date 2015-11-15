#ifndef FIND_H
#define FIND_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../../fildir.h"

int cmd_find(char *, int, char **, char *, Directory *, bool);
Directory *retrieve_dir_find(Directory *root_dir, char *name);
void print_files(Directory *, char *);
bool assert_file_name(char *, char *);

#endif
