#include "../headers/bitmap.h"

void init_bitmap()
{
  unsigned int i;
  for(i = 0; i < BITMAP_SIZE; i++) bitmap[i] = UNALLOCATED;
}
