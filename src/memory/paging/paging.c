#include "paging.h"
#include "../../status.h"
#include "../heap/kheap.h"
#include <stdint.h>

void paging_load_directory(uint32_t *directory);

static uint32_t *current_directory = 0;

paging_4gb_chunk_t *paging_new_4gb(uint8_t flags) {
  uint32_t *directory =
      kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
  size_t offset = 0;

  for (size_t i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) {
    uint32_t *entry =
        kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

    for (size_t j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; j++) {
      entry[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;
    }

    offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
    directory[i] = (uint32_t)(uintptr_t)entry | flags | PAGING_IS_WRITEABLE;
  }

  paging_4gb_chunk_t *chunk = kzalloc(sizeof(paging_4gb_chunk_t));
  chunk->directory_entry = directory;
  return chunk;
}

uint32_t *paging_4gb_chunk_get_directory(paging_4gb_chunk_t *chunk) {
  return chunk->directory_entry;
}

void paging_switch(uint32_t *directory) {
  paging_load_directory(directory);
  current_directory = directory;
}

bool paging_is_aligned(void *addr) {
  return ((uint32_t)(uintptr_t)addr % PAGING_PAGE_SIZE) == 0;
}

int paging_get_indexes(void *virtual_address, uint32_t *directory_index_out,
                       uint32_t *table_index) {
  int res = 0;

  if (!paging_is_aligned(virtual_address)) {
    res = -EINVARG;
    goto out;
  }

  *directory_index_out = ((uint32_t)(uintptr_t)virtual_address /
                          (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));

  *table_index =
      ((uint32_t)(uintptr_t)virtual_address %
       (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);

out:
  return res;
}

// val is physical address + flags
int paging_set(uint32_t *directory, void *virt, uint32_t val) {
  if (!paging_is_aligned(virt)) {
    return -EINVARG;
  }

  uint32_t directory_index = 0;
  uint32_t table_index = 0;
  int res = paging_get_indexes(virt, &directory_index, &table_index);
  if (res < 0) {
    return res;
  }

  uint32_t entry = directory[directory_index];
  uint32_t *table = (uint32_t *)(uintptr_t)(entry & 0xfffff000);
  table[table_index] = val;

  return 0;
}
