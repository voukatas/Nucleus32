#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

#define VIDEO 0xB8000

void kernel_main(void);
void print_string(char *s, uint8_t color);


#endif
