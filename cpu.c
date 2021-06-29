#include "cpu.h"
#include "utils.h"
#include "frontend/ui.h"
#include "digits.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void cpuInit(struct CPU *cpu){
	cpu->stackPointer = 0;
	cpu->programCounter = 0x200;
	cpu->stackPointer = 0xF;
	cpu->registers.I = 0;
	cpu->delayTimer = 0;
	cpu->soundTimer = 0;
	memset(cpu->registers.V, 1, 0xF);
	memset(cpu->stack, 1, 0xF);
	memset(cpu->memory.RAM, 1, 0xFFF);
	memcpy(cpu->memory.RAM, HEX_DIGITS, 0xF*0x5);
	timespec_get(&cpu->timer, NULL);
}

void step(struct CPU *cpu){
	union instructionMask im;
	im.byByte.a = *((uint8_t *)&(cpu->memory) + cpu->programCounter);
	im.byByte.b = *((uint8_t *)&(cpu->memory) + cpu->programCounter + 1);
	//im.all = ((instruction << 8) & 0xFF00) | ((instruction >> 8) & 0x00Ff);

	writeLog(LOG_LEVEL_DEBUG , "instruction: 0x%.4hx (0x%hx %hx %hx %hx) \n", im.all, im.byNibble.a, im.byNibble.b, im.byNibble.c, im.byNibble.d);
	switch(im.byNibble.a & 0x000F){
		case 0x0: 
			switch(im.byByte.b) {
				case 0x0:
					SYS(cpu, im.nnn); cpu->programCounter+=2; break;
				case 0xE0: //TODO UNREACHABLE
					clearScreen(); cpu->programCounter+=2; break;
				case 0xEE:
					RET(cpu); cpu->programCounter+=2; break;	
			} break;
		case 0x1: JP(cpu, im.nnn); break;
		case 0x2: CALL(cpu, im.nnn); cpu->programCounter+=0; break;
		case 0x3: SE_kk(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter+=2; break;
		case 0x4: SNE_kk(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter+=2;  break;
		case 0x5: SE(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
		case 0x6: LD_kk(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter+=2; break;
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
					writeLog(LOG_LEVEL_ERROR, "instruction: 0x%.4hx not found \n", im.all); 
					fflush(stdout); 
					assert(0 && "command not found");
			} break;
		case 0x9: SNE(cpu, im.byNibble.b, im.byNibble.c); cpu->programCounter+=2; break;
		case 0xA: LD_nnn(cpu, im.nnn); cpu->programCounter+=2; break;
		case 0xB: JP_v0(cpu, im.nnn); break;
		case 0xC: RND(cpu, im.byNibble.b, im.byByte.b); cpu->programCounter+=2; break;
		case 0xD: 
			drawSprite(cpu->memory.RAM + cpu->registers.I, cpu->registers.V[im.byNibble.b], cpu->registers.V[im.byNibble.c], im.byNibble.d, &cpu->registers.V[0xF]); 
							cpu->programCounter+=2; break;
		case 0xE:
			switch(im.byByte.b) {
				case 0x9E:
					SKP(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				case 0xA1:
					SKNP(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				default: 
					writeLog(LOG_LEVEL_ERROR, "instruction: 0x%.4hx not found \n", im.all); 
					fflush(stdout); 
					assert(0 && "command not found");
			} break;
		case 0xF:
			switch(im.byByte.b){
				case 0x07:
					LD_DT(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				case 0x0A:
					LD_KEY(cpu, im.byNibble.b); break;
					//writeLog(LOG_LEVEL_ERROR, "instruction %s is not implemented yet \n", "LD_KEY"); break;
				case 0x15:
					LD_SET_DT(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				case 0x18:
					LD_SET_ST(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				case 0x1E: ADD_I(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				case 0x29:
					LD_FIND_DIGIT(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				case 0x33:
					LD_BCD(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				case 0x55:
					LD_STORE_REGISTERS(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				case 0x65:
					LD_READ_REGISTERS(cpu, im.byNibble.b); cpu->programCounter+=2; break;
				default: 
					writeLog(LOG_LEVEL_ERROR, "instruction: 0x%.4hx not found \n", im.all); 
					fflush(stdout); 
					assert(0 && "command not found");
			} break;


		default: 
			writeLog(LOG_LEVEL_ERROR, "instruction: 0x%.4hx not found \n", im.all); 
			fflush(stdout); 
			assert(0 && "command not found");
	}

	usleep(1000);
	//step(cpu);
}

void handleTimers(struct CPU *cpu){
	if(cpu->delayTimer) cpu->delayTimer--;
	if(cpu->soundTimer) {
		cpu->soundTimer--;
		//beep
	}
}

//instructions

//0nnn
void SYS(struct CPU *cpu, uint16_t a){return;}
//00E0
void CLS(struct CPU *cpu){}
//00EE
void RET(struct CPU *cpu){
	cpu->programCounter = cpu->stack[cpu->stackPointer];
	cpu->stackPointer++;
}
//1nnn
void JP(struct CPU *cpu, uint16_t addr){cpu->programCounter = addr;}
//2nnn
void CALL(struct CPU *cpu, uint16_t nnn){
	cpu->stackPointer--;
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
	cpu->registers.V[a] = result & 0x00FF;
	cpu->registers.V[0xF] = result > 0xFF00; //TODO bug?
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
	cpu->registers.V[a] = cpu->registers.V[b] - cpu->registers.V[b]; //TODO bug?
}
//8xyE
void SHL(struct CPU *cpu, uint8_t a, uint8_t b){
	cpu->registers.V[0xF] = cpu->registers.V[a] & 0b10000000; //TODO bug
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
//Dxyn ========================================= TODO
//void DRW(struct CPU *, uint8_t, uint8_t, uint8_t);
//Ex9E
void SKP(struct CPU *cpu, uint8_t a){
	int8_t key;
	while((key = getch()) != ERR){
		writeLog(LOG_LEVEL_DEBUG , "key : %d, keyTable: %d \n", key, keys[a]);
		if(key == keys[cpu->registers.V[a]]){
			cpu->programCounter += 2;
			return;
		}
	}
}
//ExA1
void SKNP(struct CPU *cpu, uint8_t a){
	int8_t key;
	while((key = getch()) != ERR){
		writeLog(LOG_LEVEL_DEBUG , "key : %d, keyTable: %d  %d \n", key, keys[cpu->registers.V[a]], key == keys[cpu->registers.V[a]]);
		if(key == keys[cpu->registers.V[a]]){
			return;
		}
	}
	cpu->programCounter += 2;
}
//Fx07
void LD_DT(struct CPU *cpu, uint8_t a){
	cpu->registers.V[a] = cpu->delayTimer;
}
//Fx0A
void LD_KEY(struct CPU *cpu, uint8_t a){	
		int8_t key = getch();
		if(key != ERR){
			for(int8_t i = 0; i < 16; i++){
				if(key == keys[i]){
					cpu->registers.V[a] = i;
					cpu->programCounter+=2;
					return;
				}
			}
		}
}
//Fx15
void LD_SET_DT(struct CPU *cpu, uint8_t a){
	cpu->delayTimer = a;
}
//Fx18
void LD_SET_ST(struct CPU *cpu, uint8_t a){
	cpu->soundTimer = a;
}
//Fx1E
void ADD_I(struct CPU *cpu, uint8_t a){
	cpu->registers.I += cpu->registers.V[a];
}

//Fx29
void LD_FIND_DIGIT(struct CPU *cpu, uint8_t a){
	cpu->registers.I = cpu->registers.V[a]*0x5;
}

//Fx33
void LD_BCD(struct CPU *cpu, uint8_t a){
	uint8_t temp = cpu->registers.V[a];
	for(int i = 2; i >= 0; i--){
		cpu->memory.RAM[cpu->registers.I + i] = temp % 10;
		temp /= 10;
	}
}

//Fx55
void LD_STORE_REGISTERS(struct CPU *cpu, uint8_t a){
	for(int i = 0; i <= a; i++){
		cpu->memory.RAM[cpu->registers.I + i] = cpu->registers.V[i];  
	}
}
//Fx65
void LD_READ_REGISTERS(struct CPU *cpu, uint8_t a){
	for(int i = 0; i <= a; i++){
		cpu->registers.V[i] = cpu->memory.RAM[cpu->registers.I + i]; 
	}
}

