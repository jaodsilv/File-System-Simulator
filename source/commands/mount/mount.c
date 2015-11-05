#include "../../../headers/commands/mount/mount.h"

int cmd_mount(char *cmd, int argc, char **argv, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "mount", 5) == 0 && argc == 1) {
    if(!mounted) {
      /*If not exist, create argv as root. That is, create a directory with argv[0] path*/

      /*Else, if the system exists, don't create a new one. in this case, must think if there is anything to do*/

    }
    else
      printf("You already have a mounted file system! Unmount it first running 'umount' before mounting a new file system.\n");
    return 1;
  }
  return 0;
}
