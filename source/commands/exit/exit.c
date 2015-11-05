#include "../../../headers/commands/exit/exit.h"

/*Terminate session*/
int cmd_exit(char *cmd, bool mounted)
{
  if(strcmp(cmd, "sai") == 0 && strlen(cmd) == 3) {
    if(mounted)
      printf("You must unmount the file system before quitting. Run 'umount' command.\n");
    return 1;
  }
  return 0;
}
