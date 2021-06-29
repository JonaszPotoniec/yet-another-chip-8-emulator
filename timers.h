#ifndef _CHIP8_TIMERS_H
#define _CHIP8_TIMERS_H

#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

extern uint8_t delayTimer;
extern uint8_t soundTimer;
extern pthread_t timerThread;
extern pthread_mutex_t timerMutex;
extern _Bool timerBoolean;

int startTimers();
void stopTimers();
uint8_t readDelayTimer();
void setDelayTimer(uint8_t);
uint8_t readSoundTimer();
void setSoundTimer(uint8_t);
void timerThreadFunction();

#endif
