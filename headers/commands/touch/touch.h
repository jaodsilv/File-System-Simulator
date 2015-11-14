#ifndef TOUCH_H
#define TOUCH_H

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

int cmd_touch(char *, int, char **, char *, Directory *, bool);
int touch_file(char *, char *, Directory *);

#endif
