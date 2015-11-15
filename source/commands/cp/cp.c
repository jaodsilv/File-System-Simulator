#include "../../../headers/commands/cp/cp.h"

int cmd_cp(char *cmd, int argc, char **argv, char *file_system, Directory *root_dir, bool mounted)
{
  char bad_name[FNAME_SIZE];

  if(strncmp(cmd, "cp", 2) == 0 && argc == 2) {
    if(argv[0][0] == '/' && argv[1][0] == '/') {
      if(mounted) {
        /*If not exist, trigger error message*/
        if((fopen(file_system, "rb")) == NULL)
          printf("Unable to locate file system '%s'.\n", file_system);
        /*Else, attempt to create a new directory*/
        else {
          int ret;
          printf("Attempting to copy the file '%s' into '%s'...", argv[0], argv[1]);
          if((ret = copy_file(file_system, argv[0], argv[1], root_dir, bad_name)) == SUCCESS_CP) {
            total_files++; printf("Done!\n");
          }
          else if(ret == NO_MEM_CP)
            printf("\nThere is no remaining disk space. Operation failed.\n");
          else if(ret == BAD_PATH_R)
            printf("\nBad path: unable to find first argument '%s'. Operation failed.\n", argv[0]);
          else if(ret == BAD_PATH_S)
            printf("\nBad path: unable to find second argument '%s'. Operation failed.\n", argv[1]);
          else if(ret == TOO_LARGE_CP)
            printf("\nBad name: file name '%s' has more than 1023 characters. Operation failed.\n", argv[1]);
          else if(ret == NOT_UNIQUE_CP)
            printf("\nBad name: there is already a file named '%s'. Operation failed.\n", bad_name);
        }
      }
      else
        printf("You have no mounted file system! Mount one using 'mount <absolute_path_to_file_system_binary>'.\n");
    }
    else {
      if(argv[0][0] != '/')
        printf("Argument '%s' is not an absolute path.\n", argv[0]);
      else
        printf("Argument '%s' is not an absolute path.\n", argv[1]);
    }
    return 1;
  }
  return 0;
}

/*Attempts to copy file 'rfs_file_name' from Real File System to our file system 'fs'
with absolute name 'path_name'. file_name must be a directory path and rfs_file_name a file path*/
int copy_file(char *fs, char *rfs_file_name, char *file_name, Directory *root_dir, char *bad_name)
{
  FILE *fptr;
  unsigned int i = 0, rsize;
  Directory *p = NULL;
  char file_path[FNAME_SIZE], file_abs_name[FNAME_SIZE];

  /*Check if exists and if its a file*/
  if((fopen(rfs_file_name, "rb")) == NULL) return BAD_PATH_R;
  /*Get the size of the file*/
  fptr = fopen(rfs_file_name, "rb");
  fseek(fptr, 0, SEEK_END);
  rsize = ftell(fptr);
  fclose(fptr);

  /*Check if there is enough memory to copy the file*/
  if(!bitmap_available_memory(rsize)) return NO_MEM_CP;

  /*File names must have less than 1024 characters (including the null terminator)
    and must start and finish with '/' character*/
  file_abs_name[0] = '\0';
  if(file_name[strlen(file_name) - 1] != '/') strcat(strcpy(file_abs_name, file_name), "/");
  else strcpy(file_abs_name, file_name);

  /*Must get the file name from rfs_file_name*/
  if(rfs_file_name[strlen(rfs_file_name) - 1] != '/') {
    char fname[FNAME_SIZE];
    unsigned int j, k = 0;
    for(j = strlen(rfs_file_name) - 1; rfs_file_name[j] != '/'; j--) continue;
    for(j = j + 1; rfs_file_name[j] != '\0'; j++) fname[k++] = rfs_file_name[j];
    fname[k] = '\0';
    strcat(file_abs_name, fname);
    if(strlen(file_abs_name) + 1 >= FNAME_SIZE) return TOO_LARGE_CP;
  }
  else return BAD_PATH_R;

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

  /*Check if parent already have a file named with the choosen name.*/
  if(p != NULL) {
    File *t;
    for(t = p->f; t != NULL; t = t->next)
      if(strcmp(file_abs_name, t->name) == 0) {
        strcpy(bad_name, file_abs_name);
        return NOT_UNIQUE_CP;
      }
  }

  /*Create new file, FINALLY! :D */
  if(p == NULL) return BAD_PATH_S;
  else {
    uint16_t block_pos = 0;
    FILE *fptr2;
    File *new;
    uint16_t number[1], fat_index, next_fat_index, blocks;

    new = malloc(sizeof(*new));

    strcpy(new->name, file_abs_name);
    /*The copied file is treated as a new file in the file system*/
    get_and_format_time(new->creation);
    strcpy(new->modification, new->creation);
    strcpy(new->access, new->creation);
    new->size = rsize;
    new->fat_index  = fat_get_index();

    /*Set pointers*/
    new->prev = NULL;
    new->next = p->f;
    /*Update parent's pointers*/
    if(p->f != NULL) p->f->prev = new;
    p->f = new;

    /*Write changes to the binary*/
    fptr = fopen(fs, "r+b");
    fat_index = new->fat_index;
    /*Update FAT and Bitmap*/
    fat[fat_index] = END_OF_FILE;

    for(blocks = fat_necessary_amount_of_blocks(new->size); blocks > 0; blocks--) {
      char data[DATA_LIMIT];
      unsigned int fsize[1];
      uint16_t number[1];

      /*Get the next free block*/
      if(blocks > 1) next_fat_index = fat_get_index();
      else next_fat_index = END_OF_FILE;

      /*Update FAT and Bitmap*/
      fat[fat_index] = next_fat_index;
      bitmap[fat_index] = ALLOCATED;

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

      /*Write File info*/
      i = fat_index * BLOCK_SIZE;

      number[0] = fat_index;
      fseek(fptr, i + FD_BITMAP_INDEX, SEEK_SET);
      fwrite(number, sizeof(uint16_t), 1, fptr);
      fseek(fptr, i + FD_FAT_INDEX, SEEK_SET);
      fwrite(number, sizeof(uint16_t), 1, fptr);

      number[0] = new->fat_index;
      fseek(fptr, i + FD_FAT_FIRST_INDEX, SEEK_SET);
      fwrite(number, sizeof(uint16_t), 1, fptr);

      number[0] = fat[fat_index];
      fseek(fptr, i + FD_FAT_CONTENT, SEEK_SET);
      fwrite(number, sizeof(uint16_t), 1, fptr);

      fsize[0] = new->size;
      fseek(fptr, i + FILE_SIZE, SEEK_SET);
      fwrite(fsize, sizeof(unsigned int), 1, fptr);

      fseek(fptr, i + FD_CDATE, SEEK_SET);
      fwrite(new->creation, sizeof(char), DATE_FORMAT_SIZE, fptr);

      fseek(fptr, i + FD_MDATE, SEEK_SET);
      fwrite(new->modification, sizeof(char), DATE_FORMAT_SIZE, fptr);

      fseek(fptr, i + FD_ADATE, SEEK_SET);
      fwrite(new->access, sizeof(char), DATE_FORMAT_SIZE, fptr);

      fseek(fptr, i + FD_NAME, SEEK_SET);
      fwrite(new->name, sizeof(char), strlen(new->name) + 1, fptr);

      /*Read next data block*/
      fptr2 = fopen(rfs_file_name, "rb");
      fseek(fptr2, block_pos * DATA_LIMIT, SEEK_SET);
      fread(data, sizeof(char), DATA_LIMIT, fptr2);
      fclose(fptr2);

      /*Write data in the binary*/
      fseek(fptr, i + FILE_DATA, SEEK_SET);
      fwrite(data, sizeof(char), DATA_LIMIT, fptr);

      /*Prepare fat_index and block_pos to next iteration*/
      if(blocks > 1) {
        fat_index = fat[fat_index];
        fat[fat_index] = END_OF_FILE;
        bitmap[fat_index] = ALLOCATED;
      }
      block_pos++;
    }

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
  }
  return SUCCESS_CP;
}
