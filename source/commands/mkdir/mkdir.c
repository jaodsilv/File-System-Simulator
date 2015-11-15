#include "../../../headers/commands/mkdir/mkdir.h"

int cmd_mkdir(char *cmd, int argc, char **argv, char *file_system, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "mkdir", 5) == 0 && argc == 1) {
    if(argv[0][0] == '/') {
      if(mounted) {
        /*If not exist, trigger error message*/
        if((fopen(file_system, "rb")) == NULL)
          printf("Unable to locate file system '%s'.\n", file_system);
        /*Else, attempt to create a new directory*/
        else {
          int ret;
          printf("Attempting to create the new directory '%s'...", argv[0]);
          if((ret = create_dir(file_system, argv[0], root_dir)) == SUCCESS) {
            total_directories++; printf("Done!\n");
          }
          else if(ret == NO_MEM)
            printf("\nThere is no remaining disk space. Operation failed.\n");
          else if(ret == BAD_PATH)
            printf("\nBad path: Missing a previous directory in '%s'. Operation failed.\n", argv[0]);
          else if(ret == TOO_LARGE)
            printf("\nBad name: directory name '%s' has more than 1023 characters. Operation failed.\n", argv[0]);
          else if(ret == NOT_UNIQUE)
            printf("\nBad name: there is already a directory named '%s'. Operation failed.\n", argv[0]);
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

/*Attempts to create a directory 'dir_name'*/
int create_dir(char *fs, char *dir_name, Directory *root_dir)
{
  unsigned int i = 0;
  Directory *p = NULL;
  char dir_path[FNAME_SIZE], dir_abs_name[FNAME_SIZE];
  /*Directories occupies just 1 block with their fixed metadata, so any
    value from 1 to 4000 could be passed as argument*/
  if(!bitmap_available_memory(1)) return NO_MEM;

  /*Directory names must have less than 1024 characters (including the null terminator)
    and must start and finish with '/' character*/
  dir_abs_name[0] = '\0';
  if(dir_name[strlen(dir_name) - 1] != '/' && strlen(dir_name) + 2 < FNAME_SIZE) strcat(strcpy(dir_abs_name, dir_name), "/");
  else if(dir_name[strlen(dir_name) - 1] == '/' && strlen(dir_name) + 1 < FNAME_SIZE) strcpy(dir_abs_name, dir_name);
  else return TOO_LARGE;

  /*dir_abs_name contains the directory name. We will now extract the path
    from the absolute name and save it in dir_path*/
  get_path(dir_path, dir_abs_name);

  /*Search the tree for the parent directory of the new node*/
  p = root_dir;
  while(dir_path[i] != '\0') {
    char file_name[FNAME_SIZE];
    if(dir_path[i + 1] != '\0' && dir_path[i + 1] != '/') { i++; continue; }
    /*if its the root directory*/
    if(i == 0) break;
    /*if its another directory*/
    else if(dir_path[i + 1] == '/') {
      strncpy(file_name, dir_path, i + 2);
      file_name[i + 2] = '\0';
      /*file_name contains the directory name, with the format (example for a 'test' directory) /test/.
      NOTE: directory name will always end with '/'*/
      p = p->d;
      while(p != NULL) {
        if(strcmp(p->name, file_name) == 0) break;
        else p = p->next;
      }
      if(p == NULL) break;
      if(strcmp(p->name, dir_path) == 0) break;
      i += 2; continue;
    }
  }

  /*Check if parent already have a child directory named with the choosen name*/
  if(p != NULL) {
    Directory *t;
    for(t = p->d; t != NULL; t = t->next)
      if(strcmp(dir_abs_name, t->name) == 0)
        return NOT_UNIQUE;
  }

  /*Create new directory, FINALLY! :D */
  if(p == NULL) return BAD_PATH;
  else {
    FILE *fptr;
    Directory *new;
    uint16_t number[1];

    new = malloc(sizeof(*new));

    strcpy(new->name, dir_abs_name);
    get_and_format_time(new->creation);
    strcpy(new->modification, new->creation);
    strcpy(new->access, new->creation);
    new->fat_index  = fat_get_index();


    /*Set pointers*/
    new->f = NULL;
    new->parent = p;
    new->d = NULL;
    new->prev = NULL;
    new->next = p->d;
    /*Update parent's pointers*/
    if(p->d != NULL) p->d->prev = new;
    p->d = new;

    /*Directories consumes a single space*/
    /*Update FAT and Bitmap*/
    fat[new->fat_index] = END_OF_FILE;
    bitmap[new->fat_index] = ALLOCATED;

    /*Write changes in the binary*/
    fptr = fopen(fs, "r+b");
    /*Write free blocks*/
    number[0] = bitmap_free_blocks();
    fseek(fptr, SUPERBLOCK, SEEK_SET);
    fwrite(number, sizeof(uint16_t), 1, fptr);
    /*Write Bitmap*/
    fseek(fptr, FREE_SPACE_BLOCK, SEEK_SET);
    fwrite(bitmap, sizeof(uint8_t), BITMAP_SIZE, fptr);
    /*Write FAT*/
    fseek(fptr, FAT_BLOCK, SEEK_SET);
    fwrite(fat, sizeof(uint16_t), FAT_SIZE, fptr);
    /*Write Directory info*/
    i = new->fat_index * BLOCK_SIZE;

    number[0] = new->fat_index;
    fseek(fptr, i + FD_BITMAP_INDEX, SEEK_SET);
    fwrite(number, sizeof(uint16_t), 1, fptr);
    fseek(fptr, i + FD_FAT_INDEX, SEEK_SET);
    fwrite(number, sizeof(uint16_t), 1, fptr);
    fseek(fptr, i + FD_FAT_FIRST_INDEX, SEEK_SET);
    fwrite(number, sizeof(uint16_t), 1, fptr);

    number[0] = fat[new->fat_index];
    fseek(fptr, i + FD_FAT_CONTENT, SEEK_SET);
    fwrite(number, sizeof(uint16_t), 1, fptr);

    fseek(fptr, i + FD_CDATE, SEEK_SET);
    fwrite(new->creation, sizeof(char), DATE_FORMAT_SIZE, fptr);

    fseek(fptr, i + FD_MDATE, SEEK_SET);
    fwrite(new->modification, sizeof(char), DATE_FORMAT_SIZE, fptr);

    fseek(fptr, i + FD_ADATE, SEEK_SET);
    fwrite(new->access, sizeof(char), DATE_FORMAT_SIZE, fptr);

    fseek(fptr, i + FD_NAME, SEEK_SET);
    fwrite(new->name, sizeof(char), strlen(new->name) + 1, fptr);

    fclose(fptr);

  }
  return SUCCESS;
}
