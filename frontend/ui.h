#ifndef _CHIP8_UI_H 
#define _CHIP8_UI_H 

#include <ncurses.h>
#include <stdint.h>

WINDOW *window;
extern const int8_t keys[16];

int initUI();
void printHelloWorld();
void refreshScreen();
void endUI();
void drawSprite(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t*);

#endif
