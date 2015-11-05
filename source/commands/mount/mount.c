#include "../../../headers/commands/mount/mount.h"

int cmd_mount(char *cmd, int argc, char **argv)
{
  if(strncmp(cmd, "mount", 5) == 0 && argc == 1) {
    /*If not exist, create argv as root. That is, create a directory with argv[0] path*/

    /*Else, if the system exists, don't create a new one. in this case, must think if there is anything to do*/
    return 1;
  }
  return 0;
}
