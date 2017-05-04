// Contipede C project for SCC 110
// Main Function file

#include "curses.h"
#include "contipede_utils.h"
#include "main.h"

#define FRAMETIME 33

int main() {
	/* Start curses mode! */
	initscr();

	start_color(); // allow us to color things
	keypad(stdscr, TRUE); // allow the use of keys
	noecho(); // don't echo getch input
	curs_set(FALSE); // don't display a cursor
	cbreak(); // don't newline automatically

	// the game loop is as follows:
	while (update()) {
		draw();
	}

	endwin(); // kill ncurses

	return 0;
}

int update() {
	// handle input
	int ch = getch();

	if (ch == KEY_EXIT)
		return 0;

	cont_util_waitms(FRAMETIME);
	return 1;
}

void draw() {
	erase(); // erase the window

	

	refresh();
}