#include "heap.h"
#include <stdint.h>

static size_t align_size(size_t size);

static HEAP_BLOCK_TABLE_ENTRY heap_tbl[HEAP_DATA_POOL_NUM] = {0x00};

size_t align_size(size_t size) {
  // aligned
  if ((size % BLOCK_SIZE) == 0) {
    return size;
  }

  // needs alignment
  size_t new_size = size - (size % BLOCK_SIZE);
  new_size += BLOCK_SIZE;

  return new_size;
}
void *kmalloc(size_t size) {
  if (size < 1) {
    return NULL;
  }
  // calculate the needed blocks
  size_t aligned_size = align_size(size);
  size_t blocks_num = aligned_size / BLOCK_SIZE;

  for (size_t i = 0; i <= HEAP_DATA_POOL_NUM - blocks_num;) {

    size_t j = 0;

    for (; j < blocks_num; j++) {
      if (heap_tbl[i + j] & HEAP_BLOCK_TABLE_ENTRY_TAKEN) {
        break;
      }
    }

    if (j == blocks_num) {
      heap_tbl[i] =
          HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_TABLE_ENTRY_IS_FIRST;

      if (blocks_num > 1) {
        heap_tbl[i] |= HEAP_BLOCK_TABLE_ENTRY_HAS_NEXT;
      }

      for (size_t k = 1; k < blocks_num - 1; k++) {
        heap_tbl[i + k] =
            HEAP_BLOCK_TABLE_ENTRY_HAS_NEXT | HEAP_BLOCK_TABLE_ENTRY_TAKEN;
      }

      if (blocks_num > 1) {
        heap_tbl[i + blocks_num - 1] = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
      }

      // return the address
      return (void *)((uintptr_t)HEAP_DATA_POOL_START_ADDRESS + i * BLOCK_SIZE);
    }

    i = i + j + 1;
  }
  return NULL;
}

void kfree(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  uintptr_t addr = (uintptr_t)ptr;
  uintptr_t start = HEAP_DATA_POOL_START_ADDRESS;
  uintptr_t end = start + HEAP_DATA_POOL_NUM * BLOCK_SIZE;

  if (addr < start || addr >= end) {
    return;
  }

  // enforce block alignment
  if ((addr - start) % BLOCK_SIZE != 0) {
    return;
  }

  size_t pos = (addr - start) / BLOCK_SIZE;

  if (!(heap_tbl[pos] & HEAP_BLOCK_TABLE_ENTRY_IS_FIRST)) {
    return;
  }

  while (heap_tbl[pos] & HEAP_BLOCK_TABLE_ENTRY_HAS_NEXT) {
    heap_tbl[pos] = HEAP_BLOCK_TABLE_ENTRY_FREE;
    pos++;
  }

  heap_tbl[pos] = HEAP_BLOCK_TABLE_ENTRY_FREE;
}
