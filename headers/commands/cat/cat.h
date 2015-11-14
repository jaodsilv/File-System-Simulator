#ifndef CAT_H
#define CAT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../fat.h"
#include "../../fildir.h"
#include "../../partition.h"

#define SUCCESS        1
#define BAD_PATH_CAT   2

int cmd_cat(char *, int, char **, char *, Directory *, bool);
int cat_file(char *, char *, Directory *);

#endif
