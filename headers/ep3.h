#ifndef EP3_H
#define EP3_H
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

/*TODO:
define FAT data structures (must study FAT to see better way to implement, considering the program)
define bitmap for free space management

Not sure if these guys will stay in this header too.
*/


/*Maximum size supported by the file system simulator is 100 MB*/
#define FS_LIMIT  100

/*Prototypes*/
char *get_cmd(char *);
int expand(char *);
int get_argc(char *);
char **get_argv(char *, int, char **);
void unrecognized(char *);

#endif
