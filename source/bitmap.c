#include "../headers/bitmap.h"

/*Initializes the bitmap array*/
void bitmap_init_new()
{
  unsigned int i;
  for(i = 0; i < BITMAP_SIZE; i++) {
    if(i < FD_FIRST_BLOCK) bitmap[i] = ALLOCATED;
    else bitmap[i] = UNALLOCATED;
  }
}

/*Returns the amount of free spaces. A half allocated cluster is NOT "half free".
This "half free" is not free memory, but wasted memory*/
unsigned int total_free_space()
{
  unsigned int i, free = 0;
  for(i = 0; i < BITMAP_SIZE; i += BLOCK_SIZE)
    if(bitmap[i] == UNALLOCATED) free += BLOCK_SIZE;
  return free;
}

/*Returns the amount of wasted space*/
unsigned int total_wasted_space()
{
  unsigned int i, wasted = 0;
  for(i = 0; i < BITMAP_SIZE; i += BLOCK_SIZE) {
    if(bitmap[i] == ALLOCATED && bitmap[i + BLOCK_SIZE - 1] == UNALLOCATED) {
      unsigned int j = i + BLOCK_SIZE - 1;
      while(bitmap[j] == UNALLOCATED) { wasted++; j--; }
    }
  }
  return wasted;
}

/*Get the amount of clusters this file will need*/
unsigned int required_clusters(unsigned int file_total_size)
{
  unsigned int reqd_clusters = 0;
  do {
    reqd_clusters++;
  } while(reqd_clusters * BLOCK_SIZE < file_total_size);

  return reqd_clusters;
}
