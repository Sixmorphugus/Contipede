#include "curses.h"

#include "contipede_ship.h"

// -- SHIP GLOBAL VARIABLES --
// only accessable within this C file
int ship_x;
int ship_y;

int ship_state;

int ship_timeto_fire;

void cont_ship_init()
{
	// position the ship properly onscreen
	ship_x = getmaxx(stdscr) / 2;
	ship_y = getmaxy(stdscr) - 1;

	ship_state = ship_state_NORMAL;

	// zero all timeouts
	ship_timeto_fire = 0;
}

void cont_ship_sendch(int ch)
{
	if (ship_state != ship_state_NORMAL)
		return;
	
	// ship can do 3 things:
	// - go left
	// - go right
	// - fire

	if (ch == KEY_LEFT) {
		cont_ship_moveh(-1);
	}
	else if (ch == KEY_RIGHT) {
		cont_ship_moveh(1);
	}
	else if (ch == KEY_ENTER) {
		cont_ship_fire();
	}
}

void cont_ship_update()
{
	// keep the ship from moving offscreen

	if (ship_x >= getmaxx(stdscr)) {
		ship_x = getmaxx(stdscr)-1;
	}
	else if (ship_x < 0) {
		ship_x = 0;
	}
}

void cont_ship_setx(int x)
{
	ship_x = x;
}

void cont_ship_sety(int y)
{
	ship_y = y;
}

// shortcut function for setting both
void cont_ship_setyx(int y, int x)
{
	cont_ship_sety(y);
	cont_ship_setx(x);
}

void cont_ship_moveh(int x)
{
	ship_x += x;
}

void cont_ship_movev(int y)
{
	ship_y += y;
}

// equivelent of setyx for relative movement
void cont_ship_movevh(int v, int h)
{
	cont_ship_movev(v);
	cont_ship_moveh(h);
}

int cont_ship_getx()
{
	return ship_x;
}

int cont_ship_gety()
{
	return ship_y;
}

int ship_state_get()
{
	return ship_state;
}

void ship_state_set(int st)
{
	ship_state = st;
}

void cont_ship_draw()
{
	if (ship_state == ship_state_DEAD)
		return;

	// draw the ship onscreen
	mvprintw(ship_y, ship_x, "^");
}

void cont_ship_fire()
{
	// TODO
}
