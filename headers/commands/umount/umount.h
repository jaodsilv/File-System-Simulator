#ifndef UMOUNT_H
#define UMOUNT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../fildir.h"

int cmd_umount(char *, char *, Directory *, bool);
void free_directories(Directory *);
void free_files(File *);

#endif
