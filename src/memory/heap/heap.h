#ifndef HEAP_H
#define HEAP_H

#include "../../config.h"
#include <stdint.h>
#include <stddef.h>

// bit 7: HAS_N
// bit 6: IS_FIRST
// bit 5: UNUSED
// bit 4: UNUSED
// bit 3: ET_3
// bit 2: ET_2
// bit 1: ET_1
// bit 0: ET_0

// Calculation: heap_data_pool_start_address + (block_number * block_size)

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00
#define HEAP_BLOCK_TABLE_ENTRY_HAS_NEXT   0x80
#define HEAP_BLOCK_TABLE_ENTRY_IS_FIRST   0x40

//#define HEAP_DATA_POOL_START_ADDRESS 0x01000000
//#define HEAP_DATA_POOL_NUM 25600 // 25600 blocks * 4KB = 100MB

//#define BLOCK_SIZE 4096

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

typedef struct heap_table {
  HEAP_BLOCK_TABLE_ENTRY *entries;
  size_t total;

} heap_table_t;

typedef struct heap {
  heap_table_t *table;
  void *start_address;
} heap_t;

int heap_create(heap_t *heap, void *start_ptr, void *end_ptr,
                heap_table_t *table);

void *heap_malloc(heap_t *heap_tbl, size_t size);
void heap_free(heap_t *heap_tbl, void *ptr);


#endif // HEAP_H
