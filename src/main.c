// Contipede C project for SCC 110
// Main Function file

#include "curses.h"
#include "utils.h"
#include "main.h"

#define FRAMETIME 33
#define TESTSTRING "Hello, world!"

/* --------------- */

int hwPosition = 0;
int hwSpeed = 1;

/* --------------- */

int main() {
	/* Start curses mode! */
	initscr();

	keypad(stdscr, TRUE); // allow the use of keys
	noecho(); // don't echo getch input
	curs_set(FALSE); // Don't display a cursor

	while (update()) {
		draw();
	}

	endwin(); // kill ncurses

	return 0;
}

int update() {
	hwPosition += hwSpeed;

	if (hwPosition + strlen(TESTSTRING) > getmaxx(stdscr))
		hwSpeed = -1;

	if (hwPosition <= 0)
		hwSpeed = 1;

	u_waitms(FRAMETIME);
	return 1;
}

void draw() {
	erase(); // erase the window

	mvprintw(0, hwPosition, TESTSTRING);

	refresh();
}