#include "../../../headers/commands/umount/umount.h"

int cmd_umount(char *cmd, char *root, Directory *root_dir, bool mounted)
{
  if(strncmp(cmd, "umount", 6) == 0) {
    /*Unmount file system from this root*/
    if(mounted) {
      /*Delete every file and directory till the root*/
      kill_children(root_dir);
      /*Now reset the root info*/
      reset_root_dir(root_dir);
    }
    else
      printf("You have no mounted file system! Mount one using 'mount <absolute_path_to_file_system_root>'.\n");
    return 1;
  }
  return 0;
}

/*Reset root to prepare for a new mount call*/
void reset_root_dir(Directory *root_dir)
{
  free(root_dir->name); root_dir->name = NULL;
  root_dir->created[0] = '\0';
  root_dir->modified[0] = '\0';
  root_dir->accessed[0] = '\0';
  remove_files(root_dir);
  root_dir->d = NULL;
  root_dir->next = NULL;
  root_dir->prev = NULL;
  root_dir->children = 0;
}

/*Remove all subtrees from a given root*/
void kill_children(Directory *root)
{
  Directory *leaf, *parent;

  leaf = get_leaf(root);
  while(root->children > 0) {
    parent = leaf->parent;
    remove_directory(leaf->parent, leaf);
    leaf = parent;
    if(leaf == root && root->children > 0) leaf = get_leaf(root);
  }
}

/*Get a leaf directory*/
Directory *get_leaf(Directory *dir)
{
  if(dir->children == 0) return dir;
  else return get_leaf(dir->d);
}

/*Remove all files from a directory of the file system*/
void remove_files(Directory *dir)
{
  File *p, *q;
  for(p = dir->f; p != NULL; p = q) {
    q = p->next;
    remove_file(p, dir);
    dir->files -= 1;
  }
}

/*Remove a single file from the file system*/
void remove_file(File *p, Directory *dir)
{
  /*TODO: remove/delete file here*/
  free(p->name); p->name = NULL;
  if(dir->f == p) dir->f = p->next;
  if(p->prev != NULL) p->prev->next = p->next;
  if(p->next != NULL) p->next->prev = p->prev;
  p->prev = NULL; p->next = NULL;
  free(p); p = NULL;
}

/*Remove child directory from parent directory list. Kill child after*/
void remove_directory(Directory *parent, Directory *child)
{
  Directory *p, *q;
  for(p = parent->d; p != NULL; p = q) {
    q = p->next;
    if(child == p) {
      if(parent->d == child) parent->d = p->next;
      if(p->prev != NULL) p->prev->next = p->next;
      if(p->next != NULL) p->next->prev = p->prev;
      p->prev = NULL; p->next = NULL;
      parent->children -= 1; break;
    }
  }
  remove_files(child);
  /*TODO: remove/delete directory here*/
  free(child->name); child->name = NULL;
}
