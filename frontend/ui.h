#ifndef _CHIP8_UI_H 
#define _CHIP8_UI_H 

#include <ncurses.h>
#include <stdint.h>

WINDOW *window;

int initUI();
void printHelloWorld();
void refreshScreen();
void endUI();
void drawSprite(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t*);

#endif
