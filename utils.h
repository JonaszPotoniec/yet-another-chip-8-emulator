#ifndef _CHIP8_UTILS_H
#define _CHIP8_UTILS_H

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#define LOG_LEVEL 3
#define LOG_TO_FILE "./log.log"
//#define LOG_TO_TERMINAL

enum endiannes {
	BIG_ENDIAN,
	LITTLE_ENDIAN
};

extern FILE* logFile;

uint8_t logInit();
void logEnd();
uint8_t detectEndiannes();
uint16_t getFileSize(FILE *);
uint8_t loadRom(uint8_t *, uint16_t, char *);

enum logType {
	LOG_LEVEL_ERROR,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
};

void writeLog(uint8_t, const char*, ...);

#endif
