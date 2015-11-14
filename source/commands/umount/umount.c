#include "../../../headers/commands/umount/umount.h"

int cmd_umount(char *cmd, char *file_system, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "umount", 6) == 0) {
    /*Unmount file system from this file_system*/
    if(mounted) {
      /*TODO: free every node except the root*/
      printf("File system sucessfully unmounted!\n");
    }
    else
      printf("You have no mounted file system! Mount one using 'mount <absolute_path_to_file_system_binary>'.\n");
    return 1;
  }
  return 0;
}
