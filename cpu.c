#include "cpu.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void cpu_init(struct CPU *cpu){
	cpu->stackPointer = 0;
	cpu->programCounter = 0x200;
	cpu->stackPointer = -1;
	memset(cpu->memory.RAM, 0, 0xFFF);
}

void step(struct CPU *cpu){
	union instructionMask im;
	im.byByte.a = *((uint8_t *)&(cpu->memory) + cpu->programCounter);
	im.byByte.b = *((uint8_t *)&(cpu->memory) + cpu->programCounter + 1);
	//im.all = ((instruction << 8) & 0xFF00) | ((instruction >> 8) & 0x00Ff);

	write_log(LOG_LEVEL_DEBUG , "instruction: 0x%.4hx (0x%hx %hx %hx %hx) \n", im.all, im.byNibble.a, im.byNibble.b, im.byNibble.c, im.byNibble.d);
	switch(im.byNibble.a & 0x000F){
		case 0x0: 
			switch(im.byByte.b) {
				case 0x0:
					SYS(cpu, im.nnn); cpu->programCounter+=2; break;
				case 0xE0:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "CLS"); break;
				case 0xEE:
					RET(cpu); cpu->programCounter+=0; break;	
			} break;
		case 0x1: JP(cpu, im.nnn); break;
		case 0x2: CALL(cpu, im.nnn); cpu->programCounter+=2; break;
		case 0x3: SE_kk(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter+=2; break;
		case 0x4: SNE_kk(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter+=2;  break;
		case 0x5: SE(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
		case 0x6: LD(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter+=2; break;
		case 0x7: SUM_kk(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter+=2; break;
		case 0x8:
			switch(im.byNibble.d) {
				case 0x0: LD(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;	
				case 0x1: OR(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
				case 0x2: AND(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
				case 0x3: XOR(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
				case 0x4: SUM(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
				case 0x5: SUB(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
				case 0x6: SHR(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
				case 0x7: SUBN(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
				case 0xE: SHL(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
				default: 
					write_log(LOG_LEVEL_ERROR, "instruction: 0x%.4hx not found \n", im.all); 
					fflush(stdout); 
					assert(0 && "command not found");
			} break;
		case 0x9: SNE(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
		case 0xA: LD_nnn(cpu, im.nnn); cpu->programCounter+=2; break;
		case 0xB: JP_v0(cpu, im.nnn); break;
		case 0xC: RND(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter+=2; break;
		case 0xD: write_log(LOG_LEVEL_ERROR, "instruction %s is no implemented yet \n", "DRW"); cpu->programCounter+=2; break;
		case 0xE:
			switch(im.byByte.b) {
				case 0x9E:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "SKP"); break;
				case 0xA1:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "SKNP"); break;
				default: 
					write_log(LOG_LEVEL_ERROR, "instruction: 0x%.4hx not found \n", im.all); 
					fflush(stdout); 
					assert(0 && "command not found");
			} cpu->programCounter+=2; break;
		case 0xF:
			switch(im.byByte.b){
				case 0x07:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "LD_DT"); break;
				case 0x0A:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "LD_KEY"); break;
				case 0x15:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "LD_SET_DT"); break;
				case 0x18:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "LD_SET_ST"); break;
				case 0x1E: ADD_I(cpu, im.byByte.b); break;
				case 0x29:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "LD_FIND_DIGIT"); break;
				case 0x33:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "LD_BCD"); break;
				case 0x55:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "LD_STORE_REGISTERS"); break;
				case 0x65:
					write_log(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "LD_READ_REGISTERS"); break;
				default: 
					write_log(LOG_LEVEL_ERROR, "instruction: 0x%.4hx not found \n", im.all); 
					fflush(stdout); 
					//assert(0 && "command not found");
			} cpu->programCounter+=2; break;


		default: 
			write_log(LOG_LEVEL_ERROR, "instruction: 0x%.4hx not found \n", im.all); 
			fflush(stdout); 
			assert(0 && "command not found");
	}

	usleep(50000);
	step(cpu);
}


//instructions

//0nnn
void SYS(struct CPU *cpu, uint16_t a){return;}
//00E0
void CLS(struct CPU *cpu){}
//00EE
void RET(struct CPU *cpu){
	cpu->programCounter = cpu->stack[cpu->stackPointer];
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
	if(cpu->registers.V[a] == kk) cpu->programCounter+=2;
}
//4xy0
void SNE_kk(struct CPU *cpu, uint8_t a, uint8_t kk){
	if(cpu->registers.V[a] != kk) cpu->programCounter+=2;
}
//5xy0
void SE(struct CPU *cpu, uint8_t a, uint8_t b){
	if(cpu->registers.V[a] == cpu->registers.V[b]) cpu->programCounter+=2;
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
	if(cpu->registers.V[a] != cpu->registers.V[b]) cpu->programCounter+=2;
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
