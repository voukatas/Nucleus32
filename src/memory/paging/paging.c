#include "paging.h"
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
