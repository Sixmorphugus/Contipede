#include "contipede_moth.h"
#include "contipede_timer.h"
#include "contipede_bullet.h"
#include "contipede_platform.h"
#include "contipede_ship.h"
#include "contipede_colorpairs.h"

#include "curses.h"
#include <math.h>
#include <stdlib.h>

#define MOTH_COLOR COLOR_GREEN

// -- MOTH GLOBAL VARIABLES --
// only accessable within this C file
int moth_x;
int moth_y;

int moth_state;

int moth_timeto_fire;
int moth_timeto_move;
int moth_fired;
double moth_mvstep;

void cont_moth_init()
{
	init_pair(cont_colorpair_moth, MOTH_COLOR, COLOR_BLACK);

	moth_x = 0;
	moth_y = 0;

	moth_state = moth_state_DEAD;

	moth_timeto_fire = cont_timer_create(1000);
	moth_timeto_move = cont_timer_create(500);

	moth_fired = 0;
	moth_mvstep = 0.0;
}

void cont_moth_update()
{
	if (moth_state == moth_state_DEAD && cont_ship_get_state() != ship_state_DEAD) {
		int ch = rand() % 1000;

		if (ch == 0) {
			cont_moth_set_yx(0, rand() % getmaxx(stdscr) - 2);
			cont_moth_set_state(moth_state_NORMAL);
		}
	}
	else if (moth_state == moth_state_NORMAL) {
		if (cont_timer_finished_reset(moth_timeto_move)) {
			cont_moth_move_v(1);

			moth_mvstep += 1.0;

			double s = sin(moth_mvstep) * 5;
			cont_moth_move_h((int)floor(s));
		}

		if (moth_y > getmaxy(stdscr) - 1) {
			cont_moth_set_state(moth_state_DEAD);
		}

		cont_moth_fire();
	}
}

void cont_moth_set_x(int x)
{
	moth_x = x;
}

void cont_moth_set_y(int y)
{
	moth_y = y;
}

void cont_moth_set_yx(int y, int x)
{
	cont_moth_set_x(x);
	cont_moth_set_y(y);
}

void cont_moth_move_h(int x)
{
	moth_x += x;
}

void cont_moth_move_v(int y)
{
	moth_y += y;
}

void cont_moth_move_vh(int v, int h)
{
	cont_moth_move_h(h);
	cont_moth_move_v(v);
}

int cont_moth_get_x()
{
	return moth_x;
}

int cont_moth_get_y()
{
	return moth_y;
}

int cont_moth_get_state()
{
	return moth_state;
}

void cont_moth_set_state(int st)
{
	moth_state = st;
}

void cont_moth_draw()
{
	if (moth_state != moth_state_DEAD) {
		attron(COLOR_PAIR(cont_colorpair_moth));
		mvprintw(moth_y, moth_x - 1, "\\V/");
		attroff(COLOR_PAIR(cont_colorpair_moth));

		if (moth_fired) {
			moth_fired = 0;
			cont_plat_beep();
		}
	}
}

void cont_moth_fire()
{
	if (cont_timer_finished_reset(moth_timeto_fire)) {
		cont_bullet_create('o', 0, moth_y + 1, moth_x, 10, 0);
		moth_fired = 1;
	}
}
