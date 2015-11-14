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
