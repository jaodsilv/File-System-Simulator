#ifndef LS_H
#define LS_H

#include <stdio.h>
#include <stdbool.h>
#include "../../fildir.h"

int cmd_ls(char *, int, char **, char *, Directory *, bool);
Directory *retrieve_dir(Directory *, char *);
void list(Directory *);

#endif
