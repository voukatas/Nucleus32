#include "heap.h"
#include "../../status.h"
#include "../memory.h"
#include <stddef.h>
#include <stdint.h>

static size_t align_size(size_t size);
static int heap_validate_alignment(void *start_ptr);
static int heap_validate_table(void *start_ptr, void *end, heap_table_t *table);
#define HEAP_DATA_POOL_NUM 265000

static int heap_validate_alignment(void *ptr) {
  return (((uintptr_t)ptr % HEAP_BLOCK_SIZE) == 0);
}

static int heap_validate_table(void *start_ptr, void *end,
                               heap_table_t *table) {
  int res = 0;

  size_t table_size = (size_t)(end - start_ptr);
  size_t total_blocks = table_size / HEAP_BLOCK_SIZE;
  if (total_blocks != table->total) {
    res = -EINVARG;
    goto out;
  }

out:
  return res;
}

// static HEAP_BLOCK_TABLE_ENTRY heap_tbl[HEAP_DATA_POOL_NUM] = {0x00};

int heap_create(heap_t *heap, void *start_ptr, void *end_ptr,
                heap_table_t *table) {
  int res = 0;

  if (!heap_validate_alignment(start_ptr) ||
      !heap_validate_alignment(end_ptr)) {
    // res = -EINVARG;
    res = -1;
    goto out;
  }

  memset(heap, 0, sizeof(heap_t));
  heap->start_address = start_ptr;
  heap->table = table;

  res = heap_validate_table(start_ptr, end_ptr, table);
  if (res < 0) {
    res = -2;
    goto out;
  }

  size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
  memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
  return res;
}

static size_t align_size(size_t size) {
  // aligned
  if ((size % HEAP_BLOCK_SIZE) == 0) {
    return size;
  }

  // needs alignment
  size_t new_size = size - (size % HEAP_BLOCK_SIZE);
  new_size += HEAP_BLOCK_SIZE;

  return new_size;
}
void *heap_malloc(heap_t *heap_tbl, size_t size) {
  if (size < 1) {
    return NULL;
  }
  // calculate the needed blocks
  size_t aligned_size = align_size(size);
  uint32_t blocks_num = aligned_size / HEAP_BLOCK_SIZE;

  for (size_t i = 0; i <= HEAP_DATA_POOL_NUM - blocks_num;) {

    size_t j = 0;

    for (; j < blocks_num; j++) {
      if (heap_tbl->table->entries[i + j] & HEAP_BLOCK_TABLE_ENTRY_TAKEN) {
        break;
      }
    }

    if (j == blocks_num) {
      heap_tbl->table->entries[i] =
          HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_TABLE_ENTRY_IS_FIRST;

      if (blocks_num > 1) {
        heap_tbl->table->entries[i] |= HEAP_BLOCK_TABLE_ENTRY_HAS_NEXT;
      }

      for (size_t k = 1; k < blocks_num - 1; k++) {
        heap_tbl->table->entries[i + k] =
            HEAP_BLOCK_TABLE_ENTRY_HAS_NEXT | HEAP_BLOCK_TABLE_ENTRY_TAKEN;
      }

      if (blocks_num > 1) {
        heap_tbl->table->entries[i + blocks_num - 1] =
            HEAP_BLOCK_TABLE_ENTRY_TAKEN;
      }

      // return the address
      return (void *)((uintptr_t)HEAP_DATA_POOL_START_ADDRESS +
                      i * HEAP_BLOCK_SIZE);
    }

    i = i + j + 1;
  }
  return NULL;
}

void heap_free(heap_t *heap_tbl, void *ptr) {
  if (ptr == NULL) {
    return;
  }

  uintptr_t addr = (uintptr_t)ptr;
  uintptr_t start = HEAP_DATA_POOL_START_ADDRESS;
  uintptr_t end = start + HEAP_DATA_POOL_NUM * HEAP_BLOCK_SIZE;

  if (addr < start || addr >= end) {
    return;
  }

  // enforce block alignment
  if ((addr - start) % HEAP_BLOCK_SIZE != 0) {
    return;
  }

  size_t pos = (addr - start) / HEAP_BLOCK_SIZE;

  if (!(heap_tbl->table->entries[pos] & HEAP_BLOCK_TABLE_ENTRY_IS_FIRST)) {
    return;
  }

  while (heap_tbl->table->entries[pos] & HEAP_BLOCK_TABLE_ENTRY_HAS_NEXT) {
    heap_tbl->table->entries[pos] = HEAP_BLOCK_TABLE_ENTRY_FREE;
    pos++;
  }

  heap_tbl->table->entries[pos] = HEAP_BLOCK_TABLE_ENTRY_FREE;
}
