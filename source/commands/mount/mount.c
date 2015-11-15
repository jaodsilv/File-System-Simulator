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
          printf("The file system '%s' has been found!\nThe program will now load the file system...", argv[0]);
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
  unsigned int i;
  uint16_t free_blocks[1];

  p = fopen(fs, "rb");
  /*Free blocks*/
  fseek(p, SUPERBLOCK, SEEK_SET);
  /*Read the bitmap from the binary file*/
  fread(free_blocks, sizeof(uint16_t), 1, p);

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
  fread(root_dir->creation, sizeof(char), DATE_FORMAT_SIZE, p);
  /*Do the same for modification time*/
  fseek(p, ROOT_DIR_MODIF, SEEK_SET);
  fread(root_dir->modification, sizeof(char), DATE_FORMAT_SIZE, p);
  /*And for access time*/
  fseek(p, ROOT_DIR_ACCESS, SEEK_SET);
  fread(root_dir->access, sizeof(char), DATE_FORMAT_SIZE, p);
  /*These are root characteristics. The root is alone in the top level*/
  root_dir->parent = NULL;
  root_dir->next   = NULL;
  root_dir->prev   = NULL;
  /*For now, the root has no files or children directories*/
  root_dir->d  = NULL;
  root_dir->f  = NULL;

  if(free_blocks[0] != FRESH_FS) {
    unsigned int blocks_allocated = FRESH_FS - free_blocks[0];

    for(i = FILES_AND_SUBDIR; i < PARTITION_SIZE; i += 4000) {
      char name[FNAME_SIZE];
      bool is_a_file = false, *ptr = &is_a_file;
      unsigned int j;
      uint16_t number[1], number2[1];
      Directory dir_node[1];
      File file_node[1];

      /*First check if this block is used*/
      j = i + FD_NAME;
      fseek(p, j, SEEK_SET);
      fread(name, sizeof(char), FNAME_SIZE, p);
      /*UNUSED BLOCK*/
      if(name[0] == '\0') continue;
      /*If the tree does not contain the file, must construct every node till the file*/
      if(!tree_contains_file(root_dir, name, dir_node, file_node, ptr)) build_nodes(root_dir, name, dir_node, file_node, ptr);
      /*USED BLOCK*/
      /*Assign bitmap index as allocated*/
      j = i + FD_BITMAP_INDEX;
      fseek(p, j, SEEK_SET);
      fread(number, sizeof(uint16_t), 1, p);
      bitmap[number[0]] = ALLOCATED;
      /*Retrieve FAT index and content*/
      /*Assign FAT index with its correct content*/
      j = i + FD_FAT_INDEX;
      fseek(p, j, SEEK_SET);
      fread(number, sizeof(uint16_t), 1, p);
      j = i + FD_FAT_CONTENT;
      fseek(p, j, SEEK_SET);
      fread(number2, sizeof(uint16_t), 1, p);
      fat[number[0]] = number2[0];

      /*Assign general values to node. NOTE: The following values must be equal to all blocks of the same file*/
      if(is_a_file) {
        File *ff;

        ff = retrieve_file_node(root_dir, file_node->name);

        /*Assign first cluster*/
        j = i + FD_FAT_FIRST_INDEX;
        fseek(p, j, SEEK_SET);
        fread(number, sizeof(uint16_t), 1, p);
        ff->fat_index = number[0];

        /*Assign file size*/
        j = i + FILE_SIZE;
        fseek(p, j, SEEK_SET);
        fread(number, sizeof(uint16_t), 1, p);
        ff->size = number[0];

        /*Assign file creation date*/
        j = i + FD_CDATE;
        fseek(p, j, SEEK_SET);
        fread(ff->creation, sizeof(char), DATE_FORMAT_SIZE, p);

        /*Assign file modification date*/
        j = i + FD_MDATE;
        fseek(p, j, SEEK_SET);
        fread(ff->modification, sizeof(char), DATE_FORMAT_SIZE, p);

        /*Assign file access date*/
        j = i + FD_ADATE;
        fseek(p, j, SEEK_SET);
        fread(ff->access, sizeof(char), DATE_FORMAT_SIZE, p);
        if(--blocks_allocated == 0) break;
      }
      else {
        Directory *dd;

        dd = retrieve_dir_node(root_dir, dir_node->name);

        /*Assign first cluster*/
        j = i + FD_FAT_FIRST_INDEX;
        fseek(p, j, SEEK_SET);
        fread(number, sizeof(uint16_t), 1, p);
        dd->fat_index = number[0];

        /*Assign file creation date*/
        j = i + FD_CDATE;
        fseek(p, j, SEEK_SET);
        fread(dd->creation, sizeof(char), DATE_FORMAT_SIZE, p);

        /*Assign file modification date*/
        j = i + FD_MDATE;
        fseek(p, j, SEEK_SET);
        fread(dd->modification, sizeof(char), DATE_FORMAT_SIZE, p);

        /*Assign file access date*/
        j = i + FD_ADATE;
        fseek(p, j, SEEK_SET);
        fread(dd->access, sizeof(char), DATE_FORMAT_SIZE, p);
        if(--blocks_allocated == 0) break;
      }
    }
  }

  fclose(p);
}

void build_nodes(Directory *root_dir, char *name_from_binary, Directory *dir_node, File *file_node, bool *ptr)
{
  unsigned int i = 0;
  Directory *p = NULL;
  File *q = NULL;


  p = root_dir;
  while(name_from_binary[i] != '\0') {
    char file_name[FNAME_SIZE];

    if(name_from_binary[i + 1] != '\0' && name_from_binary[i + 1] != '/') { i++; continue; }
    /*if its a directory*/
    if(name_from_binary[i + 1] == '/') {
      Directory *parent;

      strncpy(file_name, name_from_binary, i + 2);
      file_name[i + 2] = '\0';
      /*file_name contains the directory name, with the format (example for a 'test' directory) /test/.
      NOTE: directory name will always end with '/'*/
      parent = p;
      p = p->d;
      while(p != NULL) {
        if(strcmp(p->name, file_name) == 0) break;
        else p = p->next;
      }
      /*Must build this directory node*/
      if(p == NULL) {
        Directory *new;

        new = malloc(sizeof(*new));
        strcpy(new->name, file_name);
        new->f = NULL;
        new->parent = parent;
        new->d = NULL;
        new->next = parent->d;
        new->prev = NULL;
        if(parent->d != NULL) parent->d->prev = new;
        parent->d = new;
        dir_node[0] = *new;
      }
      i += 2; continue;
    }
    /*If its a file*/
    else {
      strncpy(file_name, name_from_binary, i + 1);
      file_name[i + 1] = '\0';
      /*file_name contains the file name, with the format (example for a 'test.txt' text file) /test.txt.
      NOTE: file name will NEVER end with '/'*/
      q = p->f;
      while(q != NULL) {
        if(strcmp(q->name, file_name) == 0) break;
        else q = q->next;
      }
      /*Must build this file node*/
      if(q == NULL) {
        File *new;

        new = malloc(sizeof(*new));
        strcpy(new->name, file_name);
        new->next = p->f;
        new->prev = NULL;
        if(p->f != NULL) p->f->prev = new;
        p->f = new;
        file_node[0] = *new;
        *ptr = true;
      }
      i++; continue;
    }
  }
}

/*Check if the tree contains the file with name 'name_from_binary'*/
bool tree_contains_file(Directory *root_dir, char *name_from_binary, Directory *dir_node, File *file_node, bool *ptr)
{
  unsigned int i = 1;
  Directory *p = NULL;
  File *q = NULL;

  p = root_dir;
  while(name_from_binary[i] != '\0') {
    char file_name[1024];

    if(name_from_binary[i + 1] != '\0' && name_from_binary[i + 1] != '/') { i++; continue; }
    /*if its a directory*/
    if(name_from_binary[i + 1] == '/') {
      strncpy(file_name, name_from_binary, i + 2);
      file_name[i + 2] = '\0';
      /*file_name contains the directory name, with the format (example for a 'test' directory) /test/.
      NOTE: directory name will always end with '/'*/
      p = p->d;
      while(p != NULL) {
        if(strcmp(p->name, file_name) == 0) break;
        else p = p->next;
      }
      if(p == NULL) break;
      else if(strcmp(p->name, name_from_binary) == 0) { dir_node[0] = *p; return true; }
      i += 2; continue;
    }
    /*If its a file*/
    else {
      strncpy(file_name, name_from_binary, i + 1);
      file_name[i + 1] = '\0';
      /*file_name contains the file name, with the format (example for a 'test.txt' text file) /test.txt.
      NOTE: file name will NEVER end with '/'*/
      q = p->f;
      while(q != NULL) {
        if(strcmp(q->name, file_name) == 0) break;
        else q = q->next;
      }
      if(q == NULL) break;
      else if(strcmp(q->name, name_from_binary) == 0) { file_node[0] = *q; *ptr = true; return true; }
      i++; continue;
    }
  }
  return false;
}


/*Write recently created binary info*/
void init_binary_info(char *fs, Directory *root_dir)
{
  char time[DATE_FORMAT_SIZE];
  uint16_t number[1];
  FILE *p = NULL;

  p = fopen(fs, "wb");

  /*Write number of free blocks*/
  number[0] = TOTAL_BLOCKS - FD_FIRST_BLOCK;
  fwrite(number, sizeof(uint16_t), 1, p);

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
  fwrite(ROOT, sizeof(char), strlen(ROOT) + 1, p);
  strcpy(root_dir->name, ROOT);
  /*Move stream pointer to the next position (= root directory creation time)*/
  fseek(p, ROOT_DIR_CREAT, SEEK_SET);
  /*Write the root directory creation time into the binary file*/
  get_and_format_time(time);
  fwrite(time, sizeof(char), DATE_FORMAT_SIZE, p);
  strcpy(root_dir->creation, time);
  /*Modification time is the same of the creation time to new files/folders*/
  fseek(p, ROOT_DIR_MODIF, SEEK_SET);
  fwrite(time, sizeof(char), DATE_FORMAT_SIZE, p);
  strcpy(root_dir->modification, time);
  /*Access time is the same of the creation time to new files/folders*/
  fseek(p, ROOT_DIR_ACCESS, SEEK_SET);
  fwrite(time, sizeof(char), DATE_FORMAT_SIZE, p);
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

Directory *retrieve_dir_node(Directory *root_dir, char *name)
{
  Directory *p;
  unsigned int i = 1, last;

  /*Search the tree for the node*/
  p = root_dir;
  while(name[i] != '\0') {
    p = p->d;
    last = i;
    for(i = last; name[i] != '/' && name[i] != '\0'; i++) {
      if(name[i] == p->name[i]) continue;
      i = last - 1; p = p->next;
    }
    if(name[i] == '/') i++;
  }

  return p;
}

File *retrieve_file_node(Directory *root_dir, char *name)
{
  File *q;
  Directory *p;
  unsigned int i = 1, last;

  /*Search the tree for the node*/
  p = root_dir;
  while(name[i] != '\0') {
    p = p->d;
    last = i;
    for(i = last; name[i] != '/'; i++) {
      if(name[i] == p->name[i]) continue;
      i = last - 1; p = p->next;
    }
    i = last;
    q = p->f;
    for(i = last; name[i] != '\0' && q != NULL; i++) {
      if(name[i] == q->name[i]) continue;
      i = last - 1; q = q->next;
    }
    if(name[i] != '\0') i++;
  }

  return q;
}
