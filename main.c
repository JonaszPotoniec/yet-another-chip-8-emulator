#include <stdio.h>
//#include <stdlib.h>
#include <assert.h>
#include <signal.h>

#include "cpu.h"
#include "utils.h"
#include "frontend/ui.h"

void safeExit(){
	endUI();
	logEnd();
	exit(SIGINT);
}

int main(int argc, char *argv[]) {
	printf("starting\n");
	if(argc < 1){
		printf("specify game file!\n");
		return 0;
	}

	signal(SIGINT, safeExit);
	writeLog(LOG_LEVEL_INFO, "endiannes: %d\n", detectEndiannes());
	struct CPU cpu;
	cpuInit(&cpu);
	loadRom(cpu.memory.PROGRAM, 0xFFF - 0x200, argv[1]);
	//loadRom(cpu.memory.PROGRAM, 0xFFF - 0x200, "./test_opcode.ch8");

	if(initUI()) return 1;
	logInit();
	
	while(1){
		refreshScreen();
		step(&cpu);
	}

	safeExit(0);
	return 0;
}

