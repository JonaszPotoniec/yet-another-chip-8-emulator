#ifndef _CHIP8_CPU_H
#define _CHIP8_CPU_H

#include <stdint.h>
#include <time.h>

struct registers {
	uint8_t V[16];
	uint16_t I;
};	

struct memory {
	uint8_t RAM[0x200];
	uint8_t PROGRAM[0xFFF - 0x200];
} __attribute__ ((packed)); 

union instructionMask {
	struct {
			uint8_t d : 4;
			uint8_t c : 4;
			uint8_t b : 4;
			uint8_t a : 4;
	} __attribute__ ((packed)) byNibble;
	struct {
			uint8_t b;
			uint8_t a;
	} __attribute__ ((packed)) byByte;
	struct {
			uint16_t nnn : 12;
			uint8_t a : 4;
	} __attribute__ ((packed));
	uint16_t all;
};

struct CPU {
	struct registers registers;
	struct memory memory;
	uint16_t programCounter;
	uint16_t stack[0xF];
	uint8_t stackPointer;
	uint8_t delayTimer;
	uint8_t soundTimer;
	struct timespec timer;
};

void cpuInit(struct CPU*);
void step(struct CPU *);
void handleTimers(struct CPU*);

//instructions

//0nn
void SYS(struct CPU *, uint16_t);
//00E0
void CLS(struct CPU*);
//00EE
void RET(struct CPU*);
//1nnn
void JP(struct CPU *, uint16_t);
//2nnn
void CALL(struct CPU *, uint16_t);
//3xkk
void SE_kk(struct CPU *, uint8_t, uint8_t);
//4xy0
void SNE_kk(struct CPU *, uint8_t, uint8_t);
//5xy0
void SE(struct CPU *, uint8_t, uint8_t);
//6xkk
void LD_kk(struct CPU *, uint8_t, uint8_t);
//7xkk
void SUM_kk(struct CPU *, uint8_t, uint8_t);
//8xy0
void LD(struct CPU *, uint8_t, uint8_t);
//8xy1
void OR(struct CPU *, uint8_t, uint8_t);
//8xy2
void AND(struct CPU *, uint8_t, uint8_t);
//8xy3
void XOR(struct CPU *, uint8_t, uint8_t);
//8xy4
void SUM(struct CPU *, uint8_t, uint8_t);
//8xy5
void SUB(struct CPU *, uint8_t, uint8_t);
//8xy6
void SHR(struct CPU *, uint8_t, uint8_t);
//8xy7
void SUBN(struct CPU *, uint8_t, uint8_t);
//8xyE
void SHL(struct CPU *, uint8_t, uint8_t);
//9xy0
void SNE(struct CPU *, uint8_t, uint8_t);
//Annn
void LD_nnn(struct CPU *, uint16_t);
//Bnnn
void JP_v0(struct CPU *, uint16_t);
//Cxkk
void RND(struct CPU *, uint8_t, uint8_t);
//Dxyn
void DRW(struct CPU *, uint8_t, uint8_t, uint8_t);
//Ex9E
void SKP(struct CPU *, uint8_t);
//ExA1
void SKNP(struct CPU *, uint8_t);
//Fx07
void LD_DT(struct CPU *, uint8_t);
//Fx0A
void LD_KEY(struct CPU *, uint8_t);
//Fx15
void LD_SET_DT(struct CPU *, uint8_t);
//Fx18
void LD_SET_ST(struct CPU *, uint8_t);
//Fx1E
void ADD_I(struct CPU *, uint8_t);
//Fx29
void LD_FIND_DIGIT(struct CPU *, uint8_t);
//Fx33
void LD_BCD(struct CPU *, uint8_t);
//Fx55
void LD_STORE_REGISTERS(struct CPU *, uint8_t);
//Fx65
void LD_READ_REGISTERS(struct CPU *, uint8_t);

#endif
