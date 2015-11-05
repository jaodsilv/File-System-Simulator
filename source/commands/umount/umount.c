#include "../../../headers/commands/umount/umount.h"

int cmd_umount(char *cmd, char *root, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "umount", 6) == 0) {
    /*Unmount file system from this root*/
    if(mounted) {
      /*Delete every file and directory till the root and then delete the root*/
    }
    else
      printf("You have no mounted file system! Mount one using 'mount <absolute_path_to_file_system_root>'.\n");
    return 1;
  }
  return 0;
}
