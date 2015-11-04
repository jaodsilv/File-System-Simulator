#include "../headers/ep3.h"

int main()
{
  char *cmd = NULL, *arg = NULL;

  using_history();
  while(true) {
    if((cmd = get_cmd(cmd)) == NULL) {
      printf("Expansion attempt has failed.\n");
      free(cmd); cmd = NULL;
      if(arg != NULL) { free(arg); arg = NULL; }
      continue;
    }

    /*if(cmd_mount(cmd));
    else */if(cmd_exit(cmd)) break;
    else unrecognized(cmd);

    free(cmd); cmd = NULL;
    if(arg != NULL) { free(arg); arg = NULL; }
  }
  free(cmd); cmd = NULL;
  if(arg != NULL) { free(arg); arg = NULL; }
  printf("Program terminated.\n");
  return 0;
}

/*Get user command*/
char *get_cmd(char *cmd)
{
  cmd = readline("[EP3]: ");
  if(cmd != NULL) return (expand(cmd) != -1) ? cmd : NULL;
  else return NULL;
}

/*Expand the history or a previous command*/
int expand(char *cmd)
{
  char *expansion;
  int result;

  result = history_expand(cmd, &expansion);

  if(result == 0 || result == 1) {
    add_history(expansion);
    if(strlen(cmd) > 1) strncpy(cmd, expansion, strlen(cmd) - 1);
  }
  free(expansion); expansion = NULL;
  return result;
}

/*Get the argument for requested command 'rqst'*/
char *get_arg(char *cmd, char *arg, char *rqst)
{
  int i, j = 0;

  arg = (char*) malloc(strlen(cmd) * sizeof(*arg));
  for(i = strlen(rqst); isspace(cmd[i]); i++) continue;
  if(cmd[i] == '\0') {
    free(arg); arg = NULL;
    return NULL;
  }
  while(!isspace(cmd[i]) && cmd[i] != '\0') arg[j++] = cmd[i++];
  arg[j] = '\0';
  return arg;
}

/*User invoked unknown command to ep1sh*/
void unrecognized(char *cmd) { printf("Unrecognized command '%s'.\n", cmd); }
