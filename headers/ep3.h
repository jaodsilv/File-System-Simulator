#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
/*Include all prompt command headers*/
#include "commands/mount/mount.h"
#include "commands/cp/cp.h"
#include "commands/mkdir/mkdir.h"
#include "commands/rmdir/rmdir.h"
#include "commands/cat/cat.h"
#include "commands/touch/touch.h"
#include "commands/rm/rm.h"
#include "commands/ls/ls.h"
#include "commands/find/find.h"
#include "commands/df/df.h"
#include "commands/umount/umount.h"
#include "commands/exit/exit.h"

/*The root directory is converted to "/" in the simulation*/
#define ROOT "/"

/*Prototypes*/
char *get_cmd(char *);
int expand(char *);
int get_argc(char *);
char **get_argv(char *, int, char **);
void unrecognized(char *);
