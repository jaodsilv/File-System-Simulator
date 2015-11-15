#ifndef EP3_H
#define EP3_H
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <readline/history.h>
#include <readline/readline.h>
/*Include all prompt command headers*/
#include "fat.h"
#include "fildir.h"
#include "bitmap.h"
#include "commands/cp/cp.h"
#include "commands/df/df.h"
#include "commands/ls/ls.h"
#include "commands/rm/rm.h"
#include "commands/cat/cat.h"
#include "commands/exit/exit.h"
#include "commands/find/find.h"
#include "commands/mkdir/mkdir.h"
#include "commands/mount/mount.h"
#include "commands/rmdir/rmdir.h"
#include "commands/touch/touch.h"
#include "commands/umount/umount.h"

/*Prototypes*/
char *get_cmd(char *);
int expand(char *);
int get_argc(char *);
char **get_argv(char *, int, char **);
void unrecognized(char *);
bool cmd_time(char *, int, char **, clock_t *);

#endif
