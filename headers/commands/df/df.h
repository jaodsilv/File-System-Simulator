#ifndef DF_H
#define DF_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../fildir.h"
#include "../../partition.h"
#include "../../bitmap.h"

int cmd_df(char *, char *, bool);
void show_info(char *);

#endif
