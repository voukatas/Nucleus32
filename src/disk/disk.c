#include "disk.h"
#include "../config.h"
#include "../io/io.h"
#include "../memory/memory.h"
#include "../status.h"

disk_t disk;

int disk_read_sector(int lba, int total, void *buf) {

  outb(0x1F6, (lba >> 4) | 0xE0);
  outb(0x1F2, total);
  outb(0x1F3, (unsigned char)(lba & 0xff));
  outb(0x1F4, (unsigned char)(lba & 8));
  outb(0x1F5, (unsigned char)(lba & 16));
  outb(0x1F7, 0x20);

  unsigned short *ptr = (unsigned short *)buf;
  for (int b = 0; b < total; b++) {
    // wait for the buffer to be ready
    char c = insb(0x1F7);
    while (!(c & 0x08)) {
      c = insb(0x1F7);
    }

    // copy from hd to memory
    for (int i = 0; i < 256; i++) {
      *ptr = insw(0x1F0);
      ptr++;
    }
  }

  return 0;
}

// accepts only index 0
void disk_search_and_init() {
  memset(&disk, 0, sizeof(disk));
  disk.type = NUCLEUS32_DISK_TYPE_REAL;
  disk.sector_size = NUCLEUS32_SECTOR_SIZE;
}

disk_t *disk_get(int index) {
  if (index != 0) {
    return 0;
  }

  return &disk;
}

int disk_read_block(disk_t *idisk, unsigned int lba, int total, void *buf) {
  if (idisk != &disk) {
    return -EIO;
  }

  return disk_read_sector(lba, total, buf);
}
