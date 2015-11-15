#include "../../../headers/commands/rm/rm.h"

int cmd_rm(char *cmd, int argc, char **argv, char *file_system, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "rm", 2) == 0 && argc == 1) {
    if(argv[0][0] == '/') {
      if(mounted) {
        /*If not exist, trigger error message*/
        if((fopen(file_system, "rb")) == NULL)
          printf("Unable to locate file system '%s'.\n", file_system);
        /*Else, attempt to remove the file*/
        else {
          int ret;
          printf("Attempting to remove file '%s'...", argv[0]);
          if((ret = remove_file(file_system, argv[0], root_dir)) == SUCCESS) {
            total_files--; printf("Done!\n");
          }
          else if(ret == BAD_PATH_RM)
            printf("\nBad path: unable to locate file '%s'. Operation failed.\n", argv[0]);
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

/*Attempts to remove file 'file_name'*/
int remove_file(char *fs, char *file_name, Directory *root_dir)
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
  else return BAD_PATH_RM;

  /*file_abs_name contains the complete file name. We will now extract the path
    from the absolute name and save it in file_path*/
  get_path(file_path, file_abs_name);

  /*Search the tree for the parent directory of the file*/
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
  if(p == NULL || t == NULL) return BAD_PATH_RM;
  else {
    FILE *fptr;
    unsigned int i;
    char block[BLOCK_SIZE];
    uint16_t fat_index, next_fat_index, number[1];


    /*Update pointers. Isolate file t from the tree*/
    if(p->f == t) p->f = t->next;
    if(t->next != NULL) t->next->prev = t->prev;
    if(t->prev != NULL) t->prev->next = t->next;

    for(i = 0; i < BLOCK_SIZE; i++) block[i] = '\0';

    /*Write changes in the binary*/
    fptr = fopen(fs, "r+b");
    fat_index = t->fat_index;
    do {
      next_fat_index = fat[fat_index];

      /*Update FAT & Bitmap*/
      if(fat[fat_index] != END_OF_FILE) {
        fat[fat_index] = AVAILABLE;
        bitmap[fat_index] = UNALLOCATED;
      }

      i = fat_index * BLOCK_SIZE;

      /*i + FD_BITMAP_INDEX is the beggining of the block in the binary file*/
      fseek(fptr, i + FD_BITMAP_INDEX, SEEK_SET);
      fwrite(block, sizeof(char), BLOCK_SIZE, fptr);

      if(next_fat_index == END_OF_FILE) break;
      /*Block erased. Get next block*/
      fat_index = next_fat_index;
    } while(fat[fat_index] != END_OF_FILE);

    /*Update FAT & Bitmap*/
    fat[fat_index] = AVAILABLE;
    bitmap[fat_index] = UNALLOCATED;

    fseek(fptr, (fat_index * BLOCK_SIZE) + FD_BITMAP_INDEX, SEEK_SET);
    fwrite(block, sizeof(char), BLOCK_SIZE, fptr);

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

    fclose(fptr);

    /*Free file t*/
    free(t); t = NULL;
  }
  return SUCCESS;
}
