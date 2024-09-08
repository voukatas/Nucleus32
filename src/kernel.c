#include "kernel.h"

static void print_char(char c, uint8_t color) {
  static uint16_t *video = (uint16_t *)VIDEO;
  *video = (color << 8) | c;
  video++;
}

void print_string(char *s, uint8_t color) {
  char c;
  while ((c = *s++) != '\0') {
    print_char(c, color);
  }
}

void clear_screen() {
  uint16_t *video = (uint16_t *)VIDEO;
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 80; j++) {
      uint16_t offset = i * 80 + j;
      *(video + offset) = (0 << 8) | 'A';
    }
  }
}

void kernel_main(void) {
  clear_screen();
  print_string("Hello World!\0", 0x02);
  print_string(" \0", 0x02);
  print_string("Hello Kernel!\0", 0x02);
}
