#ifndef MOUNT_H
#define MOUNT_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../fat.h"
#include "../../fildir.h"

int cmd_mount(char *, int, char **, Directory *, bool);
void init_binary_info(char *, Directory *);
void load_binary(char *, Directory *);
bool tree_contains_file(Directory *, char *, Directory *, File *, bool *);
void build_nodes(Directory *, char *, Directory *, File *, bool *);

#endif
