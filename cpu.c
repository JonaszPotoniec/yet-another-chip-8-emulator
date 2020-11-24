#include "cpu.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void cpu_init(struct CPU *cpu){
	cpu->stackPointer = 0;
	cpu->programCounter = 0x200;
}

void step(struct CPU *cpu){
	union instructionMask im;
	im.all = *(uint16_t *)((uint8_t *)&(cpu->memory) + cpu->programCounter);

	printf("\x1B[34mDEBUG: \x1B[0minstruction: 0x%.4hx \n", im.all);
	switch(im.byNibble.a & 0x000F){
		case 0x0: SYS(cpu, im.nnn & 0x0FFF); cpu->programCounter++; break;
		case 0x1: JP(cpu, im.nnn & 0x0FFF); break;
		case 0x2: CALL(cpu, im.nnn & 0x0FFF); cpu->programCounter++; break;
		case 0x3: SE_kk(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter++; break;
		case 0x4: SNE_kk(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter++;  break;
		case 0x5: SE(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
		case 0x6: LD(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter++; break;
		case 0x7: SUM_kk(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter++; break;
		case 0x8:
			switch(im.byNibble.d) {
				case 0x0: LD(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;	
				case 0x1: OR(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
				case 0x2: AND(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
				case 0x3: XOR(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
				case 0x4: SUM(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
				case 0x5: SUB(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
				case 0x6: SHR(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
				case 0x7: SUBN(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
				case 0xE: SHL(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
			} break;
		case 0x9: SNE(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter++; break;
		case 0xA: LD_nnn(cpu, im.nnn); cpu->programCounter++; break;
		case 0xB: JP_v0(cpu, im.nnn); break;
		case 0xC: RND(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter++; break;
		case 0xD: printf("\x1B[31mERROR: \x1B[0minstruction %s is no implemented yet \n", "DRW"); cpu->programCounter++; break;
		case 0xE:
			switch(im.byByte.b) {
				case 0x9E:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "SKP"); break;
				case 0xA1:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "SKNP"); break;
			} cpu->programCounter++; break;
		case 0xF:
			switch(im.byByte.b){
				case 0x07:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "LD_DT"); break;
				case 0x0A:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "LD_KEY"); break;
				case 0x15:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "LD_SET_DT"); break;
				case 0x18:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "LD_SET_ST"); break;
				case 0x1E: ADD_I(cpu, im.byByte.b); break;
				case 0x29:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "LD_FIND_DIGIT"); break;
				case 0x33:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "LD_BCD"); break;
				case 0x55:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "LD_STORE_REGISTERS"); break;
				case 0x65:
					printf("\x1B[31mERROR: \x1B[0minstruction %s is not implemented yet \n", "LD_READ_REGISTERS"); break;
			} cpu->programCounter++; break;


		default: 
			printf("\x1B[31mERROR: \x1B[0minstruction: 0x%.4hx not found \n", im.all); 
			fflush(stdout); 
			assert(0 && "command not found");
	}

	step(cpu);
}


//instructions

//0nnn
void SYS(struct CPU *cpu, uint16_t a){return;}
//00E0
void CLS(struct CPU *cpu){}
//00EE
void RET(struct CPU *cpu){
	cpu->programCounter = cpu->stack[cpu->programCounter];
	cpu->stackPointer--;
}
//1nnn
void JP(struct CPU *cpu, uint16_t addr){cpu->programCounter = addr;}
//2nnn
void CALL(struct CPU *cpu, uint16_t nnn){
	cpu->stackPointer++;
	cpu->stack[cpu->stackPointer] = cpu->programCounter;
	cpu->programCounter = nnn;
}
//3xkk
void SE_kk(struct CPU *cpu, uint8_t a, uint8_t kk){
	if(cpu->registers.V[a] == kk) cpu->programCounter++;
}
//4xy0
void SNE_kk(struct CPU *cpu, uint8_t a, uint8_t kk){
	if(cpu->registers.V[a] != kk) cpu->programCounter++;
}
//5xy0
void SE(struct CPU *cpu, uint8_t a, uint8_t b){
	if(cpu->registers.V[a] == cpu->registers.V[b]) cpu->programCounter++;
}
//6xkk
void LD_kk(struct CPU *cpu, uint8_t a, uint8_t kk){cpu->registers.V[a] = kk;}
//7xkk
void SUM_kk(struct CPU *cpu, uint8_t a, uint8_t kk) {cpu->registers.V[a] += kk;}
//8xy0
void LD(struct CPU *cpu, uint8_t a, uint8_t b){cpu->registers.V[a] = cpu->registers.V[b];}
//8xy1
void OR(struct CPU *cpu, uint8_t a, uint8_t b){cpu->registers.V[a] |= cpu->registers.V[b];}
//8xy2
void AND(struct CPU *cpu, uint8_t a, uint8_t b){cpu->registers.V[a] &= cpu->registers.V[b];}
//8xy3
void XOR(struct CPU *cpu, uint8_t a, uint8_t b){cpu->registers.V[a] ^= cpu->registers.V[b];}
//8xy4
void SUM(struct CPU *cpu, uint8_t a, uint8_t b){
	uint16_t result = cpu->registers.V[a];
	result += cpu->registers.V[b];
	cpu->registers.V[a] = result && 0x00FF;
	cpu->registers.V[0xF] = result > 0xFF00;
}
//8xy5
void SUB(struct CPU *cpu, uint8_t a, uint8_t b){
	cpu->registers.V[0xF] = cpu->registers.V[a] > cpu->registers.V[b];
	cpu->registers.V[a] -= cpu->registers.V[b];
}
//8xy6
void SHR(struct CPU *cpu, uint8_t a, uint8_t b){
	cpu->registers.V[0xF] = cpu->registers.V[a] & 0b00000001;
	cpu->registers.V[a] /= 2;
}
//8xy7
void SUBN(struct CPU *cpu, uint8_t a, uint8_t b){
	cpu->registers.V[0xF] = cpu->registers.V[a] < cpu->registers.V[b];
	cpu->registers.V[a] = cpu->registers.V[b] - cpu->registers.V[b];
}
//8xyE
void SHL(struct CPU *cpu, uint8_t a, uint8_t b){
	cpu->registers.V[0xF] = cpu->registers.V[a] & 0b10000000;
	cpu->registers.V[a] *= 2;
}
//9xy0
void SNE(struct CPU *cpu, uint8_t a, uint8_t b){
	if(cpu->registers.V[a] != cpu->registers.V[b]) cpu->programCounter++;
}
//Annn
void LD_nnn(struct CPU *cpu, uint16_t nnn){
	cpu->registers.I = nnn;
}
//Bnnn
void JP_v0(struct CPU *cpu, uint16_t nnn){
	cpu->programCounter = (uint16_t)cpu->registers.V[0] + nnn;
}
//Cxkk
void RND(struct CPU *cpu, uint8_t a, uint8_t kk){
	cpu->registers.V[a] = rand() & kk;
}
/*
//Dxyn ========================================= TODO
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
*/
void ADD_I(struct CPU *cpu, uint8_t a){
	cpu->registers.I += cpu->registers.V[a];
}
/*
//Fx29
void LD_FIND_DIGIT(struct CPU *, uint8_t);
//Fx33
void LD_BCD(struct CPU *, uint8_t);
//Fx55
void LD_STORE_REGISTERS(struct CPU *, uint8_t);
//Fx65
void LD_READ_REGISTERS(struct CPU *, uint8_t);
*/
