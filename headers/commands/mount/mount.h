#ifndef MOUNT_H
#define MOUNT_H
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../../fat.h"

int cmd_mount(char *, int, char **, Directory *, bool);
void init_binary_info(char *, Directory *);
void load_binary(char *, Directory *);
void get_and_format_time(char *);

#endif
