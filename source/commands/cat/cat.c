#include "../../../headers/commands/cat/cat.h"

int cmd_cat(char *cmd, int argc, char **argv, char *file_system, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "cat", 3) == 0 && argc == 1) {
    if(argv[0][0] == '/') {
      if(mounted) {
        /*If not exist, trigger error message*/
        if((fopen(file_system, "rb")) == NULL)
          printf("Unable to locate file system '%s'.\n", file_system);
        /*Else, attempt to create a new directory*/
        else {
          int ret;
          printf("Attempting to cat file '%s'...", argv[0]);
          if((ret = cat_file(file_system, argv[0], root_dir)) == SUCCESS)
            printf("Done!\n");
          else if(ret == BAD_PATH_CAT)
            printf("\nBad path: unable to find text file '%s'. Operation failed.\n", argv[0]);
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

/*Attempts to cat a file 'file_name'*/
int cat_file(char *fs, char *file_name, Directory *root_dir)
{
  File *t = NULL;
  unsigned int i = 0;
  Directory *p = NULL;
  char file_path[FNAME_SIZE], file_abs_name[FNAME_SIZE];

  /*We know file names must have at most 1024 characters, including null termination character.
  therefore, a size with bigger name than the limit is obviously a bad path. It cant also be a directory*/
  file_abs_name[0] = '\0';
  if(strlen(file_name) + 1 < FNAME_SIZE && file_name[strlen(file_name) - 1] != '/')
    strcpy(file_abs_name, file_name);
  else return BAD_PATH_CAT;

  /*file_abs_name contains the complete file name. We will now extract the path
    from the absolute name and save it in file_path*/
  get_path(file_path, file_abs_name);

  /*Search the tree for the parent directory of the new node*/
  p = root_dir;
  while(file_path[i] != '\0') {
    char file_name[FNAME_SIZE];
    if(file_path[i + 1] != '\0' && file_path[i + 1] != '/') { i++; continue; }
    /*if its the root directory*/
    if(i == 0) break;
    /*if its another directory*/
    else if(file_path[i + 1] == '/') {
      strncpy(file_name, file_path, i + 2);
      file_name[i + 2] = '\0';
      /*file_name contains the directory name, with the format (example for a 'test' directory) /test/.
      NOTE: directory name will always end with '/'*/
      p = p->d;
      while(p != NULL) {
        if(strcmp(p->name, file_name) == 0) break;
        else p = p->next;
      }
      if(p == NULL) break;
      if(strcmp(p->name, file_path) == 0) break;
      i += 2; continue;
    }
  }

  /*Check if parent already have a file named with the choosen name. If yes, we got our guy*/
  if(p != NULL)
    for(t = p->f; t != NULL; t = t->next)
      if(strcmp(file_abs_name, t->name) == 0) break;

  /*Create new file, FINALLY! :D */
  if(p == NULL || t == NULL) return BAD_PATH_CAT;
  else {
    FILE *fptr;
    unsigned int i;
    uint16_t fat_index, next_fat_index;

    /*Read from the binary*/
    fptr = fopen(fs, "rb");
    fat_index = t->fat_index;

    printf("\n\n");
    printf("t = %p\n", (void*)t);
    printf("t->name = %s\n", t->name);
    printf("t->size = %u\n", t->size);
    printf("t->fat_index = %u\n", t->fat_index);
    printf("\n\n");

    do {
      char data[DATA_LIMIT + 1];

      next_fat_index = fat[fat_index];

      i = fat_index * BLOCK_SIZE;

      fseek(fptr, i + FILE_DATA, SEEK_SET);
      fread(data, sizeof(char), DATA_LIMIT, fptr);
      data[strlen(data)] = '\0';

      /*Prints data block*/
      fprintf(stderr,"%s", data);

      fat_index = next_fat_index;
    } while(fat[fat_index] != END_OF_FILE);
    fclose(fptr);
  }
  return SUCCESS;
}
