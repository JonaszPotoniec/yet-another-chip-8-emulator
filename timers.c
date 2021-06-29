#include "timers.h"
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

uint8_t delayTimer = 0;
uint8_t soundTimer = 0;
pthread_t timerThread;
pthread_mutex_t timerMutex;
_Bool timerBoolean;

int startTimers(){
	delayTimer = 0;
	soundTimer = 0;
	timerBoolean = 1;
	
	if(pthread_create(&timerThread, NULL, (void*(*)(void *))&timerThreadFunction, NULL) != 0)
		return 1;
	if(pthread_mutex_init(&timerMutex, NULL) != 0)
		return 2;
	
	return 0;
}

void stopTimers(){
	timerBoolean = 0;
	pthread_join(timerThread, NULL);
	pthread_mutex_destroy(&timerMutex);
}

uint8_t readDelayTimer(){
	pthread_mutex_lock(&timerMutex);
	uint8_t rtnVl = delayTimer;
	pthread_mutex_unlock(&timerMutex);
	
	return rtnVl;
}

void setDelayTimer(uint8_t x){
	pthread_mutex_lock(&timerMutex);
	delayTimer = x;
	pthread_mutex_unlock(&timerMutex);
}

uint8_t readSoundTimer(){
	pthread_mutex_lock(&timerMutex);
	uint8_t rtnVl = soundTimer;
	pthread_mutex_unlock(&timerMutex);
	
	return rtnVl;
}

void setSoundTimer(uint8_t x){
	pthread_mutex_lock(&timerMutex);
	soundTimer = x;
	pthread_mutex_unlock(&timerMutex);
}

void timerThreadFunction(){
	while(timerBoolean){
		pthread_mutex_lock(&timerMutex);
		if(delayTimer > 0) delayTimer--;
		if(soundTimer > 0) soundTimer--;
		pthread_mutex_unlock(&timerMutex);
		
		nanosleep((const struct timespec[]){{0, 1000000000}}, NULL);
	}
}
 
