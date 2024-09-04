	ORG 0x7c00; Instruct nasm to start offseting our labels from 0x7c00
	BITS 16

	; https://wiki.osdev.org/FAT
	; BPB (BIOS Parameter Block)
	jmp short start
	nop
	times 33 db 0

start:
	;   Initialize the Code Segment (CS) with a jmp
	jmp 0:_jmp_start

_jmp_start:
	;    Initialize the segments to avoid reading wrong addresses
	cli  ; Clear interrupts
	mov  ax, 0x00
	mov  ds, ax
	mov  es, ax
	;    Initialize the stack segment
	mov  ss, ax
	mov  sp, 0x7c00
	sti  ; Enable Interrupts
	mov  si, message
	call print
	jmp  $

print:
	mov bx, 0

.loop:
	lodsb ; Uses DS:SI (segment + offset)
	cmp   al, 0
	je    .done
	call  print_char
	jmp   .loop

.done:
	ret

print_char:
	mov ah, 0eh
	int 0x10
	ret

message:
	db    'Hello Kernel World!', 0
	times 510-($-$$) db 0
	dw    0xAA55
