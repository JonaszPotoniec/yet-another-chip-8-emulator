#include "ui.h"
#include "../utils.h"
#include <ncurses.h>
#include <string.h>

#define RESOLUTION_X 64
#define RESOLUTION_Y 32

const int8_t keys[] = {'x', '1', '2', '3', 'q', 'w', 'e', 'a', 's', 'd', 'z', 'c', '4', 'r', 'f', 'v'};

int initUI(){
	initscr();
	cbreak();
	noecho();
	timeout(1);
	int row, col;
	getmaxyx(stdscr, row, col);
	if(row < RESOLUTION_Y || col < RESOLUTION_X){
		char msg[] ="Your screen is too small";
		char msg2[] = "Try resizing your window and run this program again";
		mvprintw(row/2 - 1, (col-strlen(msg))/2, "%s", msg);
		mvprintw(row/2, (col-strlen(msg2))/2, "%s", msg2);
		getch();
		return 1;
	}
	window = newwin(RESOLUTION_Y+2, RESOLUTION_X+2, (row-RESOLUTION_Y+2)/2, (col-RESOLUTION_X+2)/2);
	if(window == NULL){
		writeLog(LOG_LEVEL_ERROR, "Can't initialize window\n");
	}
	box(window, 0, 0);
  refresh();
	wrefresh(window);
	refresh();
	return 0;
}

void printHelloWorld(){
	printw("CHIP-8 emulator");
}

void refreshScreen(){
	refresh();
}

void endUI(){
	endwin();
}

void clearScreen(){
	wclear(window);
	
	wrefresh(window);	
	refresh();
}

void drawSprite(uint8_t* addr, uint8_t x,  uint8_t y, uint8_t n, uint8_t* flag){
	*flag = 0;

	for(uint8_t byte = 0; byte < n; byte++, addr++)
		for(uint8_t bit = 0; bit < 8; bit++)
			if(*addr >> bit & 0b00000001) {
				uint8_t draw_x = (x+7-bit) % RESOLUTION_X + 1;
				uint8_t draw_y = (y+byte) % RESOLUTION_Y + 1;
				if(mvwinch(window, draw_y, draw_x) == (' '|A_REVERSE)){
					*flag = 1;
					mvwaddch(window, draw_y, draw_x, ' ');
				}
				else mvwaddch(window, draw_y, draw_x, ' '|A_REVERSE);
	}


	wrefresh(window);	
	refresh();
}

