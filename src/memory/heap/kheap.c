#include "kheap.h"
#include "../../kernel.h"
#include "heap.h"
#include <stddef.h>
#include <stdint.h>

heap_t kernel_heap;
heap_table_t kernel_heap_table;

void kheap_init() {
  size_t total_table_entries = HEAP_SIZE_BYTES / HEAP_BLOCK_SIZE;
  kernel_heap_table.entries =
      (HEAP_BLOCK_TABLE_ENTRY *)(uintptr_t)HEAP_TABLE_ADDRESS;
  kernel_heap_table.total = total_table_entries;

  void *end = (void *)(HEAP_DATA_POOL_START_ADDRESS + HEAP_SIZE_BYTES);
  int res = heap_create(&kernel_heap, (void *)HEAP_DATA_POOL_START_ADDRESS, end,
                        &kernel_heap_table);
  if (res < 0) {
    print_string("failed to init heap\n", 15);
  }

  if (res == -1) {
    print_string("failed to init heap -1\n", 15);
  }
  if (res == -2) {
    print_string("failed to init heap -2\n", 15);
  }
}

void *kmalloc(size_t size) { return heap_malloc(&kernel_heap, size); }
void kfree(void *start) { return heap_free(&kernel_heap, start); }
