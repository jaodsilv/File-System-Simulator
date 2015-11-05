#include "../../../headers/commands/mount/mount.h"

int cmd_mount(char *cmd, int argc, char **argv, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "mount", 5) == 0 && argc == 1) {
    if(argv[0][0] == '/') {
      if(!mounted) {
        printf("Attempting to mount a file system in '%s'...\n", argv[0]);
        /*If not exist, create argv as root. That is, create a directory with argv[0] path*/
        if(mkdir(argv[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
          init_root_dir(argv[0], root_dir);
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

/*Initialize a brand-new file system*/
void init_root_dir(char *root, Directory *root_dir)
{
  struct stat info;
  struct tm * date_info;

  /*Directory name*/
  root_dir->name = malloc(2 * sizeof(char));
  root_dir->name[0] = ROOT; root_dir->name[1] = '\0';

  /*Time related information (creation, access and modification)*/
  if(stat(root, &info) != 0) { printf("Error retrieving '%s' directory information.\n", root); exit(0); }
  /*New directory. All the times are assigned with the creation time*/
  date_info = localtime(&(info.st_mtime));
  strftime(root_dir->created, DATE_FORMAT_SIZE, DATE_FORMAT, date_info);
  strftime(root_dir->modified, DATE_FORMAT_SIZE, DATE_FORMAT, date_info);
  strftime(root_dir->accessed, DATE_FORMAT_SIZE, DATE_FORMAT, date_info);

  /*Set parent*/
  root_dir->parent = NULL;

  /*Set files*/
  root_dir->f = NULL;
  root_dir->files = 0;

  /*Set children directories*/
  root_dir->children_dir = NULL;
  root_dir->children = 0;
}
