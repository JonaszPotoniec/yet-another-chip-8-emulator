#include <stdio.h>

#include "cpu.h"

int main(void) {
	printf("starting\n");
	struct CPU cpu;
	cpu_init(&cpu);
	FILE *f = fopen("./MERLIN", "rb");
	fseek (f, 0, SEEK_END);
  size_t lSize = ftell (f);
  rewind (f);
	printf("\x1B[34mDEBUG: \x1B[0mCopyed: %ld bytes\n", fread(cpu.memory.PROGRAM, 1, lSize, f));
	step(&cpu);
}

