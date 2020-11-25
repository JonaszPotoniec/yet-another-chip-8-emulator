#include "utils.h"
#include <stdio.h>
#include <arpa/inet.h>

uint8_t detect_endiannes(){
	union tester {
		uint8_t nibble : 4;
		uint8_t value;
	} test;

	test.value = 0x0F;
	
	return !(test.nibble == (test.value & 0xF));
}

uint16_t getFileSize(FILE *file){
	if(!file) return 0;
	uint16_t initialPosition = ftell(file);
	fseek(file, 0, SEEK_END);
	uint16_t size = ftell(file);
	fseek(file, initialPosition, SEEK_SET);
	return size;
}

uint8_t load_rom(uint8_t *memory, uint16_t max_size, char *fileName){
	FILE *file = fopen(fileName, "rb");
	if(!file){
		write_log(LOG_LEVEL_ERROR, "Can't load file: %s\n", fileName);
		return 1;
	}

	uint16_t fileSize = getFileSize(file);
	write_log(LOG_LEVEL_DEBUG, "ROM size: %d\n", fileSize);
	if(fileSize > max_size) {
		write_log(LOG_LEVEL_ERROR, "ROM is too big, loading first %d bytes\n", max_size);
		fileSize = max_size;
	}
	//if(detect_endiannes != LITTLE_ENDIAN) {
		write_log(LOG_LEVEL_DEBUG, "Loaded %ld from %s\n",
		fread(memory, 1, fileSize, file),
		fileName);
	/*} else {
		uint16_t i;
		for(i = 0; i <= fileSize/sizeof(uint16_t); i++){
			fread((uint16_t *)memory + i, 1, sizeof(uint16_t), file);
			*((uint16_t *)memory + i) = ntohs(*((uint16_t *)memory + i));		
		}
		write_log(LOG_LEVEL_DEBUG, "Loaded %ld bytes from %s\n", (i) * sizeof(uint16_t), fileName);
	}
	*/
	fclose(file);
	return 0;
}

void write_log(uint8_t logLevel, const char* format, ...){
	#ifdef LOG_LEVEL
			if(logLevel >= LOG_LEVEL) return;
	#else
			if(logLevel > 1) return;
	#endif

	va_list args;
	va_start(args, format);

	switch(logLevel) {
		case LOG_LEVEL_ERROR:
			printf("\x1B[31m[ERROR] \x1B[0m");
			vprintf(format, args);			
			break;
		case LOG_LEVEL_INFO:
			printf("\x1B[34m[INFO] \x1B[0m");
			vprintf(format, args);			
			break;
		case LOG_LEVEL_DEBUG:
			printf("\x1B[33m[DEBUG] \x1B[0m");
			vprintf(format, args);			
			break;
		default:
			printf("\x1B[36m[UNKNOWN] \x1B[0m");
			vprintf(format, args);			
			break;
	}
}


