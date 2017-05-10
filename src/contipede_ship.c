#include "curses.h"

#include "contipede_ship.h"
#include "contipede_bullet.h"
#include "contipede_debug.h"
#include "contipede_timer.h"
#include "contipede_platform.h"

#define SHIP_FIRING_DELAY 100

// -- SHIP GLOBAL VARIABLES --
// only accessable within this C file
int ship_x;
int ship_y;

int ship_state;

int ship_timeto_fire;
int ship_fired;

void cont_ship_init()
{
	// position the ship properly onscreen
	ship_x = getmaxx(stdscr) / 2;
	ship_y = getmaxy(stdscr) - 3;

	ship_state = ship_state_NORMAL;
	ship_timeto_fire = cont_timer_create(SHIP_FIRING_DELAY);
	ship_fired = 0;

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
		cont_ship_move_h(-1);
	}
	else if (ch == KEY_RIGHT) {
		cont_ship_move_h(1);
	}
	else if (ch == KEY_UP) {
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

void cont_ship_set_x(int x)
{
	ship_x = x;
}

void cont_ship_set_y(int y)
{
	ship_y = y;
}

// shortcut function for setting both
void cont_ship_set_yx(int y, int x)
{
	cont_ship_set_y(y);
	cont_ship_set_x(x);
}

void cont_ship_move_h(int x)
{
	ship_x += x;
}

void cont_ship_move_v(int y)
{
	ship_y += y;
}

// equivelent of setyx for relative movement
void cont_ship_move_vh(int v, int h)
{
	cont_ship_move_v(v);
	cont_ship_move_h(h);
}

int cont_ship_get_x()
{
	return ship_x;
}

int cont_ship_get_y()
{
	return ship_y;
}

int ship_get_state()
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
	mvprintw(ship_y + 1, ship_x - 1, "/");
	mvprintw(ship_y + 1, ship_x, "_");
	mvprintw(ship_y + 1, ship_x + 1, "\\");

	if (ship_fired) {
		ship_fired = 0;
		cont_plat_beep();
	}
}

void cont_ship_fire()
{
	if (cont_timer_finished_reset(ship_timeto_fire)) {
		cont_bullet_create('*', 1, ship_y - 1, ship_x, -20, 0);
		ship_fired = 1;
	}
}
