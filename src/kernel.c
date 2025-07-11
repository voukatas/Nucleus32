#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"

static void print_char(char c, uint8_t color) {

  static uint16_t *video = (uint16_t *)VIDEO;
  static uint16_t row = 0;
  static uint16_t column = 0;

  if (c == '\n') {
    column = 0;
    row++;
    if (row >= VGA_HEIGHT) {
      // We just printing the last line at the bottom so we don't loose the info
      // for now
      column = 0;
      row--;
    }
    video = (uint16_t *)VIDEO + row * VGA_WIDTH + column;
    return;
  }

  *video = (color << 8) | c;
  column++;

  if (column == VGA_WIDTH) {
    column = 0;
    row++;
  }
  if (row >= VGA_HEIGHT) {
    column = 0;
    row--;
  }
  video = (uint16_t *)VIDEO + row * VGA_WIDTH + column;
}

void print_string(char *s, uint8_t color) {
  int c;
  while ((c = *s++) != '\0') {
    print_char(c, color);
  }
}

void clear_screen() {
  uint16_t *video = (uint16_t *)VIDEO;
  for (size_t i = 0; i < VGA_HEIGHT; i++) {
    for (size_t j = 0; j < VGA_WIDTH; j++) {
      uint16_t offset = i * VGA_WIDTH + j;
      *(video + offset) = (0 << 8) | ' ';
    }
  }
}

void kernel_main(void) {
  clear_screen();

  // init heap
  kheap_init();
  // init idt
  idt_init();

  // enable interrupts again
  enable_interrupts();

  print_string("Hello World!", 15);

  // test malloc/free
  void *ptr1 = kmalloc(50);
  void *ptr2 = kmalloc(5000);
  void *ptr3 = kmalloc(5600);
  kfree(ptr1);
  void *ptr4 = kmalloc(50);

  if (ptr1 || ptr2 || ptr3 || ptr4) {
  }
  // end testing
}
