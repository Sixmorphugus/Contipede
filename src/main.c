// Contipede C project for SCC 110
// Main Function file

#include "curses.h"
#include <time.h>
#include "main.h"

#include "contipede_timer.h"
#include "contipede_platform.h"
#include "contipede_ship.h"
#include "contipede_bullet.h"
#include "contipede_debris.h"
#include "contipede_centipede.h"
#include "contipede_debug.h"

int main() {
	/* Start curses mode! */
	initscr();

	start_color(); // allow us to color things
	keypad(stdscr, TRUE); // allow the use of keys
	noecho(); // don't echo getch input
	timeout(0); // don't block with getch
	curs_set(FALSE); // don't display a cursor
	cbreak(); // don't newline automatically

	cont_timers_init();
	cont_ship_init();
	cont_bullets_init();
	cont_alldebris_init();
	cont_centipedes_init();

	cont_debug_init();

	cont_debug("Contipede test version");

	cont_alldebris_generaterandom((int)time(0), cont_alldebris_get_limit());

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

	// update things
	cont_timers_update();
	cont_ship_update();
	cont_bullets_update();
	cont_alldebris_update();
	cont_centipedes_update();

	cont_debug_update();

	// wait before processing another game tick
	cont_plat_waitms(cont_plat_get_frametime());
	return 1;
}

void draw() {
	erase(); // erase the window

	cont_ship_draw();
	cont_bullets_draw();
	cont_alldebris_draw();
	cont_centipedes_draw();

	cont_debug_draw();

	refresh();
}