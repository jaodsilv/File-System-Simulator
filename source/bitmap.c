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

bool bitmap_available_memory(unsigned int size)
{
  unsigned int i, j = size;
  for(i = 0; i < BITMAP_SIZE; i++) {
    if(bitmap[i] == UNALLOCATED) {
      if(j > BLOCK_SIZE) j -= BLOCK_SIZE;
      else return true;
    }
  }
  return false;
}

uint16_t bitmap_free_blocks()
{
  uint16_t count = 0;
  unsigned int i;
  for(i = 0; i < BITMAP_SIZE; i++) if(bitmap[i] == UNALLOCATED) count++;
  return count;
}
