#ifndef IDT_H
#define IDT_H

#include <stdint.h>
typedef struct idt_desc{
	uint16_t offset_1; // Offset bits 0-15
	uint16_t selector; // selector that is GDT
	uint8_t zero; // unsued
	uint8_t type_attr; // Descriptor type and attributes
	uint16_t offset_2; // Offset bits 16-31


} __attribute__((packed)) idt_desc_t;


typedef struct idtr_desc{
	uint16_t limit; // size of descriptor table -1
	idt_desc_t *base; // Base address of the start of IDT
	//uint16_t base; // Base address of the start of IDT
}__attribute__((packed)) idtr_desc_t;

void idt_load(idtr_desc_t *ptr);
void idt_init();

#endif // IDT_H
