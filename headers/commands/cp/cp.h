#ifndef CP_H
#define CP_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../../fat.h"
#include "../../fildir.h"
#include "../../partition.h"

#define SUCCESS_CP    1
#define NO_MEM_CP     2
#define BAD_PATH_R    3
#define BAD_PATH_S    4
#define TOO_LARGE_CP  5
#define NOT_UNIQUE_CP 6

int cmd_cp(char *, int, char **, char *, Directory *, bool);
int copy_file(char *, char *, char *, Directory *, char *);

#endif
