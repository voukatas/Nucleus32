	ORG 0x7c00; Instruct nasm to start offseting our labels from 0x7c00
	BITS 16

	CODE_SEG equ gdt_code - gdt_start
	DATA_SEG equ gdt_data - gdt_start

	; https://wiki.osdev.org/FAT
	; BPB (BIOS Parameter Block)
	jmp short start
	nop
	times 33 db 0

start:
	; Initialize the Code Segment (CS) with a jmp
	jmp 0:_jmp_start
	; jmp $

_jmp_start:
	; Initialize the segments to avoid reading wrong addresses
	cli  ; Clear interrupts
	mov  ax, 0x00
	mov  ds, ax
	mov  es, ax
	; Initialize the stack segment
	mov  ss, ax
	; mov  sp, 0x7c00
	mov  sp, 0x8000
	sti  ; Enable Interrupts

	; From now on you can't use the BIOS Interrupts
.load_protected_mode:
	cli
	lgdt[gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEG:load32
	; jmp $

	;GDT
gdt_start:
	dd 0x0 ; 32-bits
	dd 0x0 ; 32-bits

	; 0x8 offset
	; GDT for code segment
gdt_code:
	dw 0xffff    ; segment length, bits 0-15
	dw 0x0       ; segment base, bits 0-15
	db 0x0       ; segment base, bits 16-23
	db 0x9a	; Access byte	
	db 11001111b ; High and low 4-bit flags
	db 0x0       ; segment base, bits 24-31

	; GDT for  DS, SS, ES, FS, GS
	; 0x10 offset
gdt_data:
	dw 0xffff
	dw 0x0
	db 0x0
	db 0x92
	db 11001111b
	db 0x0

gdt_end:

; GDT descriptor
gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
	dd gdt_start ; address (32 bit)

[BITS 32]
load32:
	mov eax,1 ; Not 0 because we don't want to load the boot sector too
	mov ecx, 100 ; We reserved that much for the kernel
	mov edi, 0x0100000 ; Where our Kernel is located
	call ata_lba_read
	jmp CODE_SEG:0x0100000


ata_lba_read:
	mov ebx, eax, ; Backup the LBA
	; Send the highest 8 bits of the lba to hard disk controller
	shr eax, 24
	or eax, 0xE0 ; Select the  master drive
	mov dx, 0x1F6
	out dx, al
	; Finished sending the highest 8 bits of the lba
	
	; Send the total sectors to read
	mov eax, ecx
	mov dx, 0x1F2
	out dx, al
	; Finished sending the total sectors to read

	; Send more bits of the LBA
	mov eax, ebx ; Restore the backup LBA
	mov dx, 0x1F3
	out dx, al
	; Finished sending more bits of the LBA

	; Send more bits of the LBA
	mov dx, 0x1F4
	mov eax, ebx ; Restore the backup LBA
	shr eax, 8
	out dx, al
	; Finished sending more bits of the LBA

	; Send upper 16 bits of the LBA
	mov dx, 0x1F5
	mov eax, ebx ; Restore the backup LBA
	shr eax, 16
	out dx, al
	; Finished sending upper 16 bits of the LBA

	mov dx, 0x1f7
	mov al, 0x20
	out dx, al

.next_sector:
	push ecx

	; Checking if we need to read
.try_again:
	mov dx, 0x1f7
	in al, dx
	test al, 8
	jz .try_again
	
	; We need to read 256 words at a time
	mov ecx, 256
	mov dx, 0x1F0
	rep insw
	pop ecx
	loop .next_sector
	; End of reading sectors into memory
	ret

	times 510-($-$$) db 0
	dw    0xAA55
