#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
#include <stdint.h>

idt_desc_t idt_descriptors[NUCLEUS32_TOTAL_INTERRUPTS];
idtr_desc_t idtr_descriptor;

void idt_zero() { print_string("Hello, divide by zero exception\n", 15); }

void idt_set(int interrupt_num, void *address) {
  idt_desc_t *descr = &idt_descriptors[interrupt_num];

  uintptr_t addr = (uintptr_t)address;

  descr->offset_1 = (uint16_t)(addr & 0x0000FFFF);
  // descr->offset_1 = (uint16_t)((uint32_t)address & 0x0000ffff);
  descr->selector = KERNEL_CODE_SELECTOR;
  descr->zero = 0x00;
  descr->type_attr = 0xEE;
  descr->offset_2 = (uint16_t)(addr >> 16);
}

void idt_init() {
  memset(idt_descriptors, -1, sizeof(idt_descriptors));

  idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
  idtr_descriptor.base = idt_descriptors;

  idt_set(0, idt_zero);

  // load interrupt descriptor table
  idt_load(&idtr_descriptor);
}
