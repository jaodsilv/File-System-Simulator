#ifndef MOUNT_H
#define MOUNT_H
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../../fildir.h"

int cmd_mount(char *, int, char **, Directory *, bool);
void init_root_dir(char *, Directory *);

#endif
