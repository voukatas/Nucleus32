#ifndef IDT_H
#define IDT_H

#include <stdint.h>
typedef struct idt_desc{
	uint16_t offset_1; // Offset bits 0-15
	uint16_t selector; // selector that is GDT
	uint16_t zero; // unsued
	uint16_t type_attr; // Descriptor type and attributes
	uint16_t offset_2; // Offset bits 16-31


} __attribute__((packed)) idt_desc_t;


typedef struct idtr_desc{
	uint16_t limit; // size of descriptor table -1
	uint32_t base; // Base address of the start of IDT
}__attribute__((packed)) idt_descr_t;

#endif // IDT_H
