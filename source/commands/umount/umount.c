#include "../../../headers/commands/umount/umount.h"

int cmd_umount(char *cmd, char *file_system, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "umount", 6) == 0) {
    /*Unmount file system from this file_system*/
    if(mounted) {
      if((fopen(file_system, "rb")) == NULL)
        printf("Unable to locate file system '%s'.\n", file_system);
      else {
        free_directories(root_dir->d);
        free_files(root_dir->f);
        total_directories = 0;
        total_files = 0;
        printf("File system sucessfully unmounted!\n");
      }
    }
    else
      printf("You have no mounted file system! Mount one using 'mount <absolute_path_to_file_system_binary>'.\n");
    return 1;
  }
  return 0;
}

void free_directories(Directory *dir) {
  if (dir == NULL) return;
  free_directories(dir->d);
  free_directories(dir->next);
  free_files(dir->f);
  free(dir);
}

void free_files(File *f) {
  if (f == NULL) return;
  free_files(f->next);
  free(f);
}
