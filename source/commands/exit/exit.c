#include "../../../headers/commands/exit/exit.h"

/*Terminate session*/
int cmd_exit(char *cmd)
{
  if(strcmp(cmd, "sai") == 0 && strlen(cmd) == 3) return 1;
  return 0;
}
