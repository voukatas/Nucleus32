#include "kernel.h"

void kernel_main(void) {
  char *video_memory = (char *)0xB8000;
  video_memory[0] = 'H';
  video_memory[1] = 2;
  video_memory[2] = 'e';
  video_memory[3] = 2;
  video_memory[4] = 'l';
  video_memory[5] = 2;
  video_memory[6] = 'l';
  video_memory[7] = 2;
  video_memory[8] = 'o';
  video_memory[9] = 2;
}
