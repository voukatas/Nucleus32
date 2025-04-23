#include "idt.h"
#include "config.h"

idt_desc_t idt_descriptors[NUCLEUS32_TOTAL_INTERRUPTS];
