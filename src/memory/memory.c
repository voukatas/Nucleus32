#include "memory.h"

void *memset(void *ptr, int c, size_t size) {

  unsigned char *ptr_c = (unsigned char *)ptr;

  for (size_t i = 0; i < size; i++) {
    ptr_c[i] = (unsigned char)c;
  }

  return ptr;
}
