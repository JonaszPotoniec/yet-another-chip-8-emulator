#include <stdio.h>
//#include <stdlib.h>
#include <assert.h>
#include <signal.h>

#include "cpu.h"
#include "utils.h"
#include "frontend/ui.h"

void save_exit(int exitCode){
	endUI();
	log_end();
	exit(exitCode);
}

int main(void) {
	printf("starting\n");

	signal(SIGINT, save_exit);
	write_log(LOG_LEVEL_INFO, "endiannes: %d\n", detect_endiannes());
	struct CPU cpu;
	cpu_init(&cpu);
	load_rom(cpu.memory.PROGRAM, 0xFFF - 0x200, "./MERLIN");
	//load_rom(cpu.memory.PROGRAM, 0xFFF - 0x200, "./test_opcode.ch8");

	if(initUI()) return 1;
	log_init();
	
	while(1){
		refreshScreen();
		step(&cpu);
	}

	save_exit(0);
	return 0;
}

