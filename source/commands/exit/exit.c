#include "../../../headers/commands/exit/exit.h"

/*Terminate session*/
int cmd_exit(char *cmd, bool *mounted)
{
  if(strcmp(cmd, "sai") == 0 && strlen(cmd) == 3) {
    if(*mounted) {
      while(true) {
        char *decision = NULL;
        printf("You are leaving the simulator without unmounting the file system. Quit simulation anyway? [Y/N]\n");
        decision = readline("> ");
        if((decision[0] == 'y' || decision[0] == 'Y') && strlen(decision) == 1) {
          *mounted = false; break;
        }
        else if((decision[0] == 'n' || decision[0] == 'N') && strlen(decision) == 1)
          break;
      }
    }
    return 1;
  }
  return 0;
}
