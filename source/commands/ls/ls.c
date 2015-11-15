#include "../../../headers/commands/ls/ls.h"

int cmd_ls(char *cmd, int argc, char **argv, char *file_system, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "ls", 2) == 0 && argc == 1) {
    if(argv[0][0] == '/') {
      if(mounted) {
        /*If not exist, trigger error message*/
        if((fopen(file_system, "rb")) == NULL)
          printf("Unable to locate file system '%s'.\n", file_system);
        /*Else, attempt to list files*/
        else {
          Directory *listed = retrieve_dir(root_dir, argv[0]);
          printf("Attempting to list '%s'...\n", argv[0]);
          list(listed);
        }
      }
      else
        printf("You have no mounted file system! Mount one using 'mount <absolute_path_to_file_system_binary>'.\n");
    }
    else
      printf("Argument '%s' is not an absolute path.\n", argv[0]);
    return 1;
  }
  return 0;
}

Directory *retrieve_dir(Directory *root_dir, char *name)
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

void list(Directory *dir)
{
	if (dir->f != NULL) {
		File *f;
		printf("File\t-\tSize(bytes)\t-\tLast modification\n");
		for(f = dir->f; f != NULL; f = f->next) {
			printf("%s\t-\t%u\t-\t%s\n", f->name, f->size, f->modification);
		}
		printf("\n");
	}
	if (dir->d != NULL) {
		printf("Directory\t-\tLast modification\n");
		for(dir = dir->d; dir != NULL; dir = dir->next) {
			printf("%s\t-\t%s", dir->name, dir->modification);
		}
		printf("\n");
	}
}