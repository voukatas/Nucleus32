export PREFIX := $(HOME)/opt/cross
export TARGET := i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

FILES=build/kernel.asm.o build/kernel.o
INCLUDES= -I ./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -Wextra


all: bin/boot.bin bin/kernel.bin
	rm -rf bin/os.bin
	dd if=bin/boot.bin of=bin/os.bin
	dd if=bin/kernel.bin >> bin/os.bin
	# reserve space for our kernel about 512KB
	dd if=/dev/zero bs=512 count=100 >> bin/os.bin

run:
	qemu-system-x86_64 -hda bin/os.bin

bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o build/kernel_full.o
	i686-elf-gcc $(FLAGS) -T src/linker.ld -o bin/kernel.bin build/kernel_full.o

bin/boot.bin: src/boot/boot.asm
	nasm -f bin src/boot/boot.asm -o bin/boot.bin


build/kernel.asm.o: src/kernel.asm
	nasm -f elf -g src/kernel.asm -o build/kernel.asm.o

build/kernel.o: src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

clean:
	rm -rf bin/boot.bin
	rm -rf bin/os.bin
	rm -rf bin/kernel.bin
	rm -rf $(FILES)
	rm -rf build/kernel_full.o
