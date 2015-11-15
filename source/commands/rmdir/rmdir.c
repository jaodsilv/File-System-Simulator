#include "../../../headers/commands/rmdir/rmdir.h"

int cmd_rmdir(char *cmd, int argc, char **argv, char *file_system, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "rmdir", 5) == 0 && argc == 1) {
    if(argv[0][0] == '/') {
      if(mounted) {
        /*If not exist, trigger error message*/
        if((fopen(file_system, "rb")) == NULL)
          printf("Unable to locate file system '%s'.\n", file_system);
        /*Else, attempt to remove the directory and everything content under it*/
        else {
          int ret;
          printf("Attempting to remove directory '%s'...", argv[0]);
          if((ret = remove_directory(file_system, argv[0], root_dir)) == SUCCESS)
            printf("\nDone! All files have been deleted!\n");
          else if(ret == BAD_PATH_RMDIR)
            printf("\nBad path: unable to locate file '%s'. Operation failed.\n", argv[0]);
          else if(ret == FORBIDDEN)
            printf("\nForbidden operation detected: you are not allowed to remove the root!\n");
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

/*Attempts to remove file 'dir_name'*/
int remove_directory(char *fs, char *dir_name, Directory *root_dir)
{
  Directory *t = NULL;
  unsigned int i = 0;
  Directory *p = NULL;
  char dir_path[FNAME_SIZE], dir_abs_name[FNAME_SIZE];

  /*Can't remove the root directory*/
  if(strcmp(dir_name, ROOT) == 0) return FORBIDDEN;

  /*We know file names must have at most 1024 characters, including null termination character.
  therefore, a size with bigger name than the limit is obviously a bad path. It cant also be a directory*/
  dir_abs_name[0] = '\0';
  if(dir_name[strlen(dir_name) - 1] != '/' && strlen(dir_name) + 2 < FNAME_SIZE) strcat(strcpy(dir_abs_name, dir_name), "/");
  else if(dir_name[strlen(dir_name) - 1] == '/' && strlen(dir_name) + 1 < FNAME_SIZE) strcpy(dir_abs_name, dir_name);
  else return BAD_PATH_RMDIR;

  /*dir_abs_name contains the complete file name. We will now extract the path
    from the absolute name and save it in dir_path*/
  get_path(dir_path, dir_abs_name);

  /*Search the tree for the parent directory of the file*/
  p = root_dir;
  while(dir_path[i] != '\0') {
    char dir_name[FNAME_SIZE];
    if(dir_path[i + 1] != '\0' && dir_path[i + 1] != '/') { i++; continue; }
    /*if its the root directory*/
    if(i == 0) break;
    /*if its another directory*/
    else if(dir_path[i + 1] == '/') {
      strncpy(dir_name, dir_path, i + 2);
      dir_name[i + 2] = '\0';
      /*dir_name contains the directory name, with the format (example for a 'test' directory) /test/.
      NOTE: directory name will always end with '/'*/
      p = p->d;
      while(p != NULL) {
        if(strcmp(p->name, dir_name) == 0) break;
        else p = p->next;
      }
      if(p == NULL) break;
      if(strcmp(p->name, dir_path) == 0) break;
      i += 2; continue;
    }
  }

  /*Check if parent already have a directory named with the choosen name. If yes, we got our guy*/
  if(p != NULL)
    for(t = p->d; t != NULL; t = t->next)
      if(strcmp(dir_abs_name, t->name) == 0) break;

  /*Remove directory, FINALLY! :D */
  if(p == NULL || t == NULL) return BAD_PATH_RMDIR;
  else {
    /*Update pointers. Isolate directory t from the tree*/
    if(p->d == t) p->d = t->next;
    if(t->next != NULL) t->next->prev = t->prev;
    if(t->prev != NULL) t->prev->next = t->next;

    while(t != NULL) {
      Directory *leaf;

      /*Get leaf directory*/
      leaf = get_leaf(t);
      /*Isolate leaf from the tree*/
      if(leaf->parent->d == leaf) leaf->parent->d = leaf->next;
      /*Remove files from leaf directory*/
      remove_files(fs, leaf, leaf->f);
      /*Remove directory*/
      remove_dir(fs, leaf);
      if(t == leaf) { free(t); t = NULL; }
      else { free(leaf); leaf = NULL; }
      total_directories--;
    }
  }
  return SUCCESS;
}

Directory *get_leaf(Directory *dd)
{
  if(dd->d == NULL) return dd;
  else return get_leaf(dd->d);
}

/*Remove all files from directory p*/
void remove_files(char *fs, Directory *p, File *t)
{
  FILE *fptr;
  unsigned int i;
  char block[BLOCK_SIZE];
  uint16_t fat_index, next_fat_index, number[1];

  while(t != NULL) {
    printf("\nRemoving file '%s'... ", t->name);

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
    printf("Done!");
    free(t); t = p->f;
    total_files--;
  }
}

/*Remove directory t*/
void remove_dir(char *fs, Directory *t)
{
  FILE *fptr;
  unsigned int i;
  char block[BLOCK_SIZE];
  uint16_t fat_index, next_fat_index, number[1];


  printf("\nRemoving directory '%s'... ", t->name);

  /*Update pointers.*/
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
  printf("Done!");
}
