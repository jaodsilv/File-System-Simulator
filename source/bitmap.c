#include "../headers/bitmap.h"

/*Initializes the bitmap array*/
void bitmap_init()
{
  unsigned int i;
  for(i = 0; i < BITMAP_SIZE; i++) bitmap[i] = UNALLOCATED;
}

/*Returns the amount of free spaces. A half allocated cluster is NOT "half free".
This "half free" is not free memory, but wasted memory*/
unsigned int total_free_space()
{
  unsigned int i, free = 0;
  for(i = 0; i < BITMAP_SIZE; i += CLUSTER_SIZE)
    if(bitmap[i] == UNALLOCATED) free += CLUSTER_SIZE;
  return free;
}

/*Returns the amount of wasted space*/
unsigned int total_wasted_space()
{
  unsigned int i, wasted = 0;
  for(i = 0; i < BITMAP_SIZE; i += CLUSTER_SIZE) {
    if(bitmap[i] == ALLOCATED && bitmap[i + CLUSTER_SIZE - 1] == UNALLOCATED) {
      unsigned int j = i + CLUSTER_SIZE - 1;
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
  } while(reqd_clusters * CLUSTER_SIZE < file_total_size);

  return reqd_clusters;
}

/*Looks for 'required_clusters' contiguous memory. Dont call this function. Call file_allocated() instead*/
unsigned int get_starting_index(unsigned int required_clusters)
{
  bool found = false;
  unsigned int i;
  for(i = 0; i < BITMAP_SIZE - (CLUSTER_SIZE * (required_clusters - 1)); i += CLUSTER_SIZE) {
    if(bitmap[i] == UNALLOCATED && bitmap[i + (CLUSTER_SIZE * (required_clusters - 1))] == UNALLOCATED) {
      unsigned int j = i + (CLUSTER_SIZE * (required_clusters - 1));
      while(bitmap[j] == UNALLOCATED && j > i) j -= CLUSTER_SIZE;
      if(i == j) { found = true; break; }
      else i = j - CLUSTER_SIZE;
    }
  }
  if(found) return i;
  else return BITMAP_SIZE;
}

/*Allocate a file. Don't call this function directly. Call file_allocated() in a condition instead*/
void allocate_file(unsigned int starting_index, unsigned int file_total_size)
{
  unsigned int i;
  for(i = starting_index; i < starting_index + file_total_size; i++) bitmap[i] = ALLOCATED;
}

/*Attempts to allocate a file and return if action was succesful*/
/*Depending on the answer (if true), fat will then call its method to store file*/
bool file_allocated(File *f)
{
  unsigned int bitmap_index = get_starting_index(required_clusters(f->total_size));
  if(bitmap_index < BITMAP_SIZE) {
    f->bitmap_index = bitmap_index;
    allocate_file(f->bitmap_index, f->total_size);
    return true;
  }
  return false;
}

/*Free a file. Don't call this function directly. Call file_freed() in a condition instead*/
void free_file(unsigned int starting_index, unsigned int file_total_size)
{
  unsigned int i;
  for(i = starting_index; i < starting_index + file_total_size; i++) bitmap[i] = UNALLOCATED;
}

/*Attempts to allocate a file and return if action was succesful*/
/*Depending on the answer (if true), fat will then call its method to unstore file*/
bool file_freed(File *f)
{
  free_file(f->bitmap_index, f->total_size);
  return true;
}
