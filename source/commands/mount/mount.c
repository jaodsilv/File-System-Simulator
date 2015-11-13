#include "../../../headers/commands/mount/mount.h"

int cmd_mount(char *cmd, int argc, char **argv, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "mount", 5) == 0 && argc == 1) {
    if(argv[0][0] == '/') {
      if(!mounted) {

        printf("Attempting to mount file system '%s'...\n", argv[0]);
        /*If not exist, creates a new binary*/
        if((fopen(argv[0], "rb")) == NULL) {
          printf("Unable to find file system '%s'.\nInitializing a new file system... ", argv[0]);
          /*Write file system fresh info*/
          init_binary_info(argv[0], root_dir);
          printf("Done!\n");
        }
        /*Else, then it exists. Must read & load the binary*/
        else {
          printf("The file system '%s' has been found!\nThe program will now load the file system... ", argv[0]);
          load_binary(argv[0], root_dir);
          printf("Done!\n");
        }
      }
      else
        printf("You already have a mounted file system! Unmount it first running 'umount' before mounting a new file system.\n");
    }
    else
      printf("Argument '%s' is not an absolute path.\n", argv[0]);
    return 1;
  }
  return 0;
}

/*Loads a binary file system previously created*/
void load_binary(char *fs, Directory *root_dir)
{
  FILE *p = NULL;

  p = fopen(fs, "rb");

  /*Load bitmap*/
  /*Move stream pointer to the wanted position (= bitmap position)*/
  fseek(p, FREE_SPACE_BLOCK, SEEK_SET);
  /*Read the bitmap from the binary file*/
  fread(bitmap, sizeof(uint8_t), BITMAP_SIZE, p);

  /*Load FAT*/
  /*Move stream pointer to another position (= fat position)*/
  fseek(p, FAT_BLOCK, SEEK_SET);
  /*Initialize and read the fat from the binary file*/
  fread(fat, sizeof(uint16_t), FAT_SIZE, p);

  /*Load root*/
  /*Move stream pointer to another position (root directory name)*/
  fseek(p, ROOT_DIR_NAME, SEEK_SET);
  /*Read the root directory name from the binary file*/
  fread(root_dir->name, sizeof(char), 2, p);
  /*Move stream pointer to another position (root directory creation time)*/
  fseek(p, ROOT_DIR_CREAT, SEEK_SET);
  /*Read the root directory creation time from the binary file*/
  fread(root_dir->creation, sizeof(char), 20, p);
  /*Do the same for modification time*/
  fseek(p, ROOT_DIR_MODIF, SEEK_SET);
  fread(root_dir->modification, sizeof(char), 20, p);
  /*And for access time*/
  fseek(p, ROOT_DIR_ACCESS, SEEK_SET);
  fread(root_dir->access, sizeof(char), 20, p);
  /*These are root characteristics. The root is alone in the top level*/
  root_dir->parent = NULL;
  root_dir->next   = NULL;
  root_dir->prev   = NULL;
  /*For now, the root has no files or children directories*/
  root_dir->d  = NULL;
  root_dir->f  = NULL;

  /*TODO: loop now here constructing the directory tree with the rest of the informations in the binary file*/
  fclose(p);
}

/*Write recently created binary info*/
void init_binary_info(char *fs, Directory *root_dir)
{
  char time[20];
  unsigned int number[1];
  FILE *p = NULL;

  p = fopen(fs, "wb");

  /*Write number of free blocks*/
  number[0] = TOTAL_BLOCKS - FD_FIRST_BLOCK;
  fwrite(number, sizeof(unsigned int), 1, p);

  /*Initialize bitmap*/
  /*Move stream pointer to the next position (= bitmap position)*/
  fseek(p, FREE_SPACE_BLOCK, SEEK_SET);
  /*Initialize and write the bitmap into the binary file*/
  bitmap_init_new();
  fwrite(bitmap, sizeof(uint8_t), BITMAP_SIZE, p);

  /*Initialize FAT*/
  /*Move stream pointer to the next position (= fat position)*/
  fseek(p, FAT_BLOCK, SEEK_SET);
  /*Initialize and write the fat into the binary file*/
  fat_init_new();
  fwrite(fat, sizeof(uint16_t), FAT_SIZE, p);

  /*Initialize root directory*/
  /*Move stream pointer to the next position (= root directory name)*/
  fseek(p, ROOT_DIR_NAME, SEEK_SET);
  /*Write the root directory name into the binary file*/
  fwrite(ROOT, sizeof(char), 2, p);
  strcpy(root_dir->name, ROOT);
  /*Move stream pointer to the next position (= root directory creation time)*/
  fseek(p, ROOT_DIR_CREAT, SEEK_SET);
  /*Write the root directory creation time into the binary file*/
  get_and_format_time(time);
  fwrite(time, sizeof(char), 20, p);
  strcpy(root_dir->creation, time);
  /*Modification time is the same of the creation time to new files/folders*/
  fseek(p, ROOT_DIR_MODIF, SEEK_SET);
  fwrite(time, sizeof(char), 20, p);
  strcpy(root_dir->modification, time);
  /*Access time is the same of the creation time to new files/folders*/
  fseek(p, ROOT_DIR_ACCESS, SEEK_SET);
  fwrite(time, sizeof(char), 20, p);
  strcpy(root_dir->access, time);

  /*File must have (partition size)bytes*/
  fseek(p, PARTITION_SIZE, SEEK_SET);
  fputc('\n', p);
  fclose(p);

  /*These are root characteristics. The root is alone in the top level*/
  root_dir->parent = NULL;
  root_dir->next   = NULL;
  root_dir->prev   = NULL;
  /*For now, the root has no files or children directories*/
  root_dir->d  = NULL;
  root_dir->f  = NULL;
}

/*Get the local time and write it in the following format: DD/MM/YYYY HH:MM:SS*/
void get_and_format_time(char *output)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime); timeinfo = localtime(&rawtime);
    sprintf(output, "%d/%d/%d %d:%d:%d",timeinfo->tm_mday, timeinfo->tm_mon + 1,
      timeinfo->tm_year + 1900, timeinfo->tm_hour,
      timeinfo->tm_min, timeinfo->tm_sec);
}
