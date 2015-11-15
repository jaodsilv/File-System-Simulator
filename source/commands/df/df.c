#include "../../../headers/commands/df/df.h"

int cmd_df(char *cmd, char *file_system, bool mounted)
{
  if(strncmp(cmd, "df", 2) == 0) {
    /*Show file_system information*/
    if(mounted) {
      printf("Fetching information...\n");
      show_info(file_system);
    }
    else
      printf("You have no mounted file system! Mount one using 'mount <absolute_path_to_file_system_binary>'.\n");
    return 1;
  }
  return 0;
}

void show_info(char *file_system)
{
  FILE *p;
  char data[DATA_LIMIT];
  unsigned int free = 0, wasted = 0, i;

  free = bitmap_free_blocks();
  free *= BLOCK_SIZE;

  p = fopen(file_system, "rb");

  for(i = FILES_AND_SUBDIR; i < PARTITION_SIZE; i += BLOCK_SIZE) {
    if(bitmap[i / BLOCK_SIZE] == ALLOCATED) {
      unsigned int j;
      fseek(p, i + FILE_DATA, SEEK_SET);
      fread(data, sizeof(char), DATA_LIMIT, p);
      for(j = 0; j < DATA_LIMIT; j++) if(data[j] == '\0') wasted++;
    }
  }

  fclose(p);

  printf("Free Space: %u bytes\n", free);
  printf("Wasted Space: %u bytes\n", wasted - (total_directories * DATA_LIMIT));
  printf("Directories: %u\n", total_directories);
  printf("Files: %u\n", total_files);
}
