#include <stdio.h>

#include "cpu.h"
#include "utils.h"


int main(void) {
	printf("starting\n");
	write_log(LOG_LEVEL_INFO, "endiannes: %d\n", detect_endiannes());
	struct CPU cpu;
	cpu_init(&cpu);
	//FILE *f = fopen("./MERLIN", "rb");
	//fseek (f, 0, SEEK_END);
  //size_t lSize = ftell (f);
  //rewind (f);
	//write_log(LOG_LEVEL_DEBUG, "Copied: %ld bytes\n", fread(cpu.memory.PROGRAM, 1, lSize, f));
	load_rom(cpu.memory.PROGRAM, 0xFFF - 0x200, "./MERLIN");
	step(&cpu);
}

