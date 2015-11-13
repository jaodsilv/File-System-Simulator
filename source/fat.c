#include "../headers/fat.h"

/*Initializes file allocation table*/
void fat_init_new()
{
  unsigned int i;
  for(i = 0; i < FAT_SIZE; i++) {
    if(i < FD_FIRST_BLOCK) fat[i] = RESERVED;
    else fat[i] = AVAILABLE;
  }
}
