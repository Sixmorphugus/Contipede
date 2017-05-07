// Contipede C project for SCC 110
// Main Function file

#include "curses.h"
#include "main.h"

#include "contipede_utils.h"
#include "contipede_ship.h"

#define FRAMETIME 33

int main() {
	/* Start curses mode! */
	initscr();

	start_color(); // allow us to color things
	keypad(stdscr, TRUE); // allow the use of keys
	noecho(); // don't echo getch input
	timeout(0); // don't block with getch
	curs_set(FALSE); // don't display a cursor
	cbreak(); // don't newline automatically

	cont_ship_init();

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

	cont_ship_sendch(ch);

	cont_ship_update();

	cont_util_waitms(FRAMETIME);
	return 1;
}

void draw() {
	erase(); // erase the window

	cont_ship_draw();

	refresh();
}
