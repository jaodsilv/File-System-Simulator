#include "../headers/fat.h"

/*Initializes file allocation table*/
void fat_init()
{
  int i;
  for(i = 0; i < FAT_SIZE; i++) fat[i] = NOT_IN_USE;
}

/*Stores the file in the allocation table*/
void fat_store(File *f)
{
  bool first = true;
  int i, first_index, last_assigned;
  unsigned int j, reqd_clusters = 0;

  /*Get the number of required clusters*/
  do {
    reqd_clusters++;
  } while(reqd_clusters * CLUSTER_SIZE < f->total_size);

  for(i = 0, j = 0; i < FAT_SIZE && j < reqd_clusters; i++) {
    if(fat[i] == NOT_IN_USE && first) {
      first = false; j++;
      last_assigned = first_index = i;
    }
    else if(fat[i] == NOT_IN_USE && !first) {
      j++; fat[last_assigned] = i;
      last_assigned = i;
    }
    if(reqd_clusters == j) fat[last_assigned] = END_OF_FILE;
  }

  f->fat_index = first_index;
}

/*Frees the file in the allocation table*/
void fat_free(File *f)
{
  int i;
  for(i = f->fat_index; fat[i] != END_OF_FILE; i = fat[i]) fat[i] = NOT_IN_USE;
  fat[i] = NOT_IN_USE;
}
