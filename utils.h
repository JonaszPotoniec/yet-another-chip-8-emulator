#ifndef _CHIP8_UTILS_H
#define _CHIP8_UTILS_H

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#define LOG_LEVEL 3

enum endiannes {
	BIG_ENDIAN,
	LITTLE_ENDIAN
};

uint8_t detect_endiannes();
uint16_t getFileSize(FILE *);
uint8_t load_rom(uint8_t *, uint16_t, char *);

enum logType {
	LOG_LEVEL_ERROR,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
};

void write_log(uint8_t, const char*, ...);

#endif
