#include "../headers/ep3.h"

int main()
{
  Directory *root_dir;
  bool mounted = false, *ptrm = &mounted;
  int argc; char **argv = NULL;
  char *cmd = NULL, *root = NULL;

  root_dir = malloc(sizeof(*root_dir));
  /*Initialize bitmap. Set all positions to UNALLOCATED status*/
  init_bitmap();
  /*Initialize fat. Set all positions to NOT_IN_USE status*/
  init_fat();
  /*Prompt history*/
  fat_use(1, 3999);
  fat_use(0, 4000);
  using_history();
  while(true) {
    /*Get prompt requested command*/
    if((cmd = get_cmd(cmd)) == NULL) {
      printf("Expansion attempt has failed.\n");
      free(cmd); cmd = NULL;
      continue;
    }

    /*Get arguments from requested copmmand*/
    argc = get_argc(cmd);
    argv = get_argv(cmd, argc, argv);

    /*Do file system request command*/
    if(cmd_mount(cmd, argc, argv, root_dir, mounted)) {
      if(argv[0][0] == '/') {
        if(root != NULL) free(root);
        root = malloc((strlen(argv[0]) + 1) * sizeof(*root));
        strcpy(root, argv[0]);
        mounted = true;
      }
    }
    else if(cmd_umount(cmd, root, root_dir, mounted)) { mounted = false; }
    else if(cmd_exit(cmd, ptrm)) { if(!mounted) break; }
    else unrecognized(cmd);

    /*Free for the next iteration*/
    free(cmd); cmd = NULL;
    if(argv != NULL) {
      int i;
      for(i = 0; i < argc; i++) { free(argv[i]); argv[i] = NULL; }
      free(argv); argv = NULL;
    }
  }
  free(cmd); cmd = NULL;
  if(root != NULL) { free(root); root = NULL; }
  if(argv != NULL) {
    int i;
    for(i = 0; i < argc; i++) { free(argv[i]); argv[i] = NULL; }
    free(argv); argv = NULL;
  }

  free(root_dir); root_dir = NULL;
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

/*Get the number of arguments of the command*/
int get_argc(char *cmd)
{
  unsigned int i; int argc = 0;

  for(i = 0; i < strlen(cmd); i++)
    if(isspace(cmd[i])) {
      while(isspace(cmd[i])) {
        if(cmd[++i] == '\0') break;
        else if(!isspace(cmd[i])) { argc++; break; }
      }
      i--;
    }
  return argc;
}

/*Get the arguments of the given command*/
char **get_argv(char *cmd, int argc, char **argv)
{
  /*If there are arguments...*/
  if(argc > 0) {
    int i, arg_count = 0;
    /*Allocate memory for the argc arguments*/
    argv = malloc(argc * sizeof(char *));
    /*Move to an index after the requested command*/
    for(i = 0; !isspace(cmd[i]); i++) continue;
    /*Get arguments*/
    while(arg_count < argc) {
      int j, start, end, k;
      /*search for the next argument starting index*/
      while(isspace(cmd[i])) i++;
      /*Mark argument starting index*/
      start = i;
      /*Search argument ending index*/
      while(!isspace(cmd[i])) {
        i++;
        if(cmd[i] == '\0') break;
      }
      /*Mark argument ending index*/
      end = i;
      /*Allocate the argument size*/
      argv[arg_count] = malloc((end - start + 1) * sizeof(char));
      /*Copy argument to argv*/
      for(j = 0, k = start; j < end - start; j++, k++) argv[arg_count][j] = cmd[k];
      argv[arg_count++][j] = '\0';
    }
  }
  return argv;
}

/*User invoked unknown command to ep1sh*/
void unrecognized(char *cmd) { printf("Unrecognized command '%s'.\n", cmd); }

/*Test Function*/
void print_root_dir_info(Directory *root_dir, bool mounted)
{
  if(mounted) {
    printf("My Root Directory:\n");
    printf("Name: '%s'\n", root_dir->name);
    printf("Creation Date: '%s'\n", root_dir->created);
    printf("Modified Date: '%s'\n", root_dir->modified);
    printf("Accessed Date: '%s'\n", root_dir->accessed);
    printf("Parent: '%p'\n", (void*)root_dir->parent);
    printf("Head File: '%p'\n", (void*)root_dir->f);
    printf("Total files: '%d'\n", root_dir->files);
    printf("Head Child Directory: '%p'\n", (void*)root_dir->d);
    printf("Total Children Directories: '%d'\n", root_dir->children);
  }
}
