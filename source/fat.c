#include "../headers/fat.h"
#include <stdio.h>

void init_fat()
{
  int i;
  for(i = 0; i < FAT_SIZE; i++) fat[i] = NOT_IN_USE;
}

void fat_use(int fat_index, unsigned int bitmap_index)
{
  if(fat[fat_index] == NOT_IN_USE)
    fat[fat_index] = BLOCK(bitmap_index);
}

void fat_release(int fat_index)
{
  if(IN_USE(fat[fat_index]))
    fat[fat_index] = NOT_IN_USE;
}
