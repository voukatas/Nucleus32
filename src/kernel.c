#include "kernel.h"

static void print_char(char c, uint8_t color) {
  static uint16_t *video = (uint16_t *)VIDEO;
  static uint16_t row = 0;
  static uint16_t column = 0;
  if (c == '\n') {
    uint16_t offset = (VGA_WIDTH - column);
    video = video + offset;
    return;
  }
  *video = (color << 8) | c;
  video++;
  column++;
  if (column == VGA_WIDTH) {
    column = 0;
    row++;
  }
}

size_t strlen(const char *s) {
  size_t len = 0;
  while ((*s++)) {
    len++;
  }

  return len;
}

void print_string(char *s, uint8_t color) {
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) {
    print_char(s[i], color);
  }
  // char c;
  // while ((c = *s++) != '\0') {
  //   print_char(c, color);
  // }
}

void clear_screen() {
  uint16_t *video = (uint16_t *)VIDEO;
  for (size_t i = 0; i < VGA_HEIGHT; i++) {
    for (size_t j = 0; j < VGA_WIDTH; j++) {
      uint16_t offset = i * VGA_WIDTH + j;
      *(video + offset) = (0 << 8) | 'A';
    }
  }
}

void kernel_main(void) {
  clear_screen();
  print_string("Hello World!\0", 15);
  print_string("\n\0", 15);
  print_string("Hello Kernel!\0", 15);
}
