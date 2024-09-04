# Nucleus32

**Nucleus32** is a minimalist 32-bit operating system kernel for the x86 architecture. It is designed as a learning tool and a base for further development in low-level programming and OS concepts.

## Getting Started

### Prerequisites

To build and run Nucleus32, you will need:

- GCC cross-compiler
- `qemu` for emulation and testing
- `make` and `nasm`

### Building Nucleus32

Clone the repository:

```bash
git clone https://github.com/yourusername/Nucleus32.git
cd Nucleus32
nasm -f bin boot.asm -o boot.bin
qemu-system-x86_64 -hda ./boot.bin
```

### Debugging
```bash
# Check the disassembled code
ndisasm boot.bin
```
