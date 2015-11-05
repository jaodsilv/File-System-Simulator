#include "../../../headers/commands/mount/mount.h"

int cmd_mount(char *cmd, int argc, char **argv, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "mount", 5) == 0 && argc == 1) {
    if(argv[0][0] == '/') {
      if(!mounted) {
        printf("Attempting to mount a file system in '%s'...\n", argv[0]);
        /*If not exist, create argv as root. That is, create a directory with argv[0] path*/
        if(mkdir(argv[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {

          printf("Your new file system is mounted! '%s' will now be referenced in this simulation as '/'.\n", argv[0]);
        }
        /*Else, if the system exists, don't create a new one. in this case, must think if there is anything to do*/
        else {
          printf("A file system is already mounted in '%s'! Reading its contents...\n", argv[0]);
          /*TODO: read contents and store data in data structure*/
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

void init_root_dir(Directory *root_dir)
{
  root_dir.name = malloc(2 * sizeof(*name));
  root_dir.name[0] = ROOT; root_dir.name[1] = '\0';
  
}

typedef struct directory {
  char *name;                       /*Directory name*/
  char *created;                    /*Creation date*/
  char *modified;                   /*Last modification date*/
  char *accessed;                   /*Last access date*/
  struct directory *parent;         /*Parent of this directory. If root, then parent = NULL*/
  /*File list*/
  File *f;                          /*"Head" file. If directory is empty, f = NULL*/
  unsigned int files;               /*Total number of files in the list (and thus, in this directory)*/
  struct directory *children_dir;   /*Array with all directories that are direct children of this directory.*/
  unsigned int children;            /*Number of children directories (children_dir size). mkdir calls must start setting this value to zero*/
} Directory;
