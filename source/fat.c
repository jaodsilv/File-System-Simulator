#include "../headers/fat.h"

/*Initializes file allocation table*/
void fat_init_new()
{
  uint16_t i;
  for(i = 0; i < FAT_SIZE; i++) {
    if(i < FD_FIRST_BLOCK) fat[i] = RESERVED;
    else fat[i] = AVAILABLE;
  }
}

uint16_t fat_get_index()
{
  uint16_t i;
  for(i = 0; i < FAT_SIZE; i++) if(fat[i] == AVAILABLE) break;
  return i;
}

uint16_t fat_necessary_amount_of_blocks(unsigned int size)
{
  uint16_t blocks_needed = 0;
  unsigned int j = size;

  while(1) {
    if(j >= DATA_LIMIT) { j -= DATA_LIMIT; blocks_needed++; }
    else break;
  }

  if(j == 0) return blocks_needed;
  else return blocks_needed + 1;
}
