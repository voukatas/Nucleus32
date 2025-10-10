#ifndef DISK_H
#define DISK_H

typedef unsigned int NUCLEUS32_DISK_TYPE;

// real physical hard disk
#define NUCLEUS32_DISK_TYPE_REAL 0

typedef struct disk {
  NUCLEUS32_DISK_TYPE type;
  int sector_size;
} disk_t;

//int disk_read_sector(int lba, int total, void *buf);
void disk_search_and_init();
disk_t *disk_get(int index);
int disk_read_block(disk_t *idisk, unsigned int lba, int total, void *buf);

#endif // DISK_H
