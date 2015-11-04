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

/*Prototypes*/
char *get_cmd(char *);
int expand(char *);
char *get_arg(char *, char *, char *);
void unrecognized(char *);
