#include "../../../headers/commands/find/find.h"

int cmd_find(char *cmd, int argc, char **argv, char *file_system, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "find", 4) == 0 && argc == 2) {
    if(argv[0][0] == '/') {
      if(mounted) {
        /*If not exist, trigger error message*/
        if((fopen(file_system, "rb")) == NULL)
          printf("Unable to locate file system '%s'.\n", file_system);
        /*Else, attempt to find argv[1]*/
        else {
          Directory *dir = retrieve_dir_find(root_dir, argv[0]);
          if (dir != NULL) print_files(dir, argv[1]);
          else printf("Directory '%s' not found.\n", argv[0]);
        }
      }
      else
        printf("You have no mounted file system! Mount one using 'mount <absolute_path_to_file_system_binary>'.\n");
    }
    else {
      printf("Argument '%s' is not an absolute path.\n", argv[0]);
    }
    return 1;
  }
  return 0;
}

Directory *retrieve_dir_find(Directory *root_dir, char *name)
{
  Directory *p;
  unsigned int i = 1, last;

  /*Search the tree for the node*/
  p = root_dir;
  while(name[i] != '\0') {
    p = p->d;
    last = i;
    for(i = last; name[i] != '/' && name[i] != '\0'; i++) {
      if(name[i] == p->name[i]) continue;
      i = last - 1; p = p->next;
    }
    if(name[i] == '/') i++;
  }

  return p;
}

void print_files(Directory *root_dir, char *name)
{
  File *q;
  Directory *p;

  /*Search the tree for the node*/
  for(q = root_dir->f; q != NULL; q = q->next) {
    if (assert_file_name(name, q->name) == 0) printf("%s\n", q->name);
  }

  for(p = root_dir->d; p != NULL; p = p->next) {
    print_files(p, name);
  }
}

bool assert_file_name(char *name1, char *name2)
{
  char *fis = strrchr(name2, '/')+1;
  return strcmp(name1, fis);
}