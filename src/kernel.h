#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

#define VIDEO 0xB8000

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void kernel_main(void);
//void print_string(char *s, uint8_t color);
size_t strlen(const char *s);


#endif
