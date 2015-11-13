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
