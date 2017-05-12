#include "curses.h"

#include <stdlib.h>

#include "contipede_centipede.h"
#include "contipede_colorpairs.h"
#include "contipede_timer.h"
#include "contipede_platform.h"
#include "contipede_debug.h"
#include "contipede_debris.h"

// some constants!
#define CENTIPEDE_LIMIT 100
#define CENTIPEDE_LENGTH_LIMIT 100
#define CENTIPEDE_START_LENGTH 10
#define CENTIPEDE_DEATH_LENGTH 3

#define CENTIPEDE_COLOR COLOR_GREEN

typedef struct {
	int used;

	int x, y;
	double basespeed;
	int mvDir;
	int length;
	int moveTimer;

	// these positions need to be initially filled in with the starting position of the centipede
	int prevX[CENTIPEDE_LENGTH_LIMIT];
	int prevY[CENTIPEDE_LENGTH_LIMIT];
} centipede_data;

int centipedes_next_length;
centipede_data centipede_data_array[CENTIPEDE_LIMIT];

void cont_centipedes_init()
{
	init_pair(cont_colorpair_centipede, CENTIPEDE_COLOR, COLOR_BLACK);

	for (int i = 0; i < CENTIPEDE_LIMIT; i++) {
		centipede_data_array[i].used = 0;
	}

	cont_centipedes_reset_next_length();
}

int cont_centipede_create(int y, int x, int movedir, int length, double basespeed)
{
	for (int i = 0; i < CENTIPEDE_LIMIT; i++) {
		if (!cont_centipede_exists(i)) {
			if (length > CENTIPEDE_LENGTH_LIMIT) {
				//cont_debug("Given centipede length is illegal - trimmed");
				length = CENTIPEDE_LENGTH_LIMIT;
			}

			centipede_data_array[i].basespeed = basespeed;
			centipede_data_array[i].length = length;
			centipede_data_array[i].mvDir = movedir;
			centipede_data_array[i].y = y;
			centipede_data_array[i].x = x;

			centipede_data_array[i].moveTimer = cont_timer_create(cont_plat_timeout_from_speed(cont_centipede_speed(basespeed, length)));

			if (centipede_data_array[i].moveTimer == -1) {
				cont_debug("Failed to create a new centipede - timer limit hit");
			}

			for (int j = 0; j < CENTIPEDE_LENGTH_LIMIT; j++) {
				centipede_data_array[i].prevY[j] = y;
				centipede_data_array[i].prevX[j] = x;
			}

			centipede_data_array[i].used = 1;

			return i;
		}
	}

	cont_debug("Failed to create a new centipede - limit hit");
	return -1;
}

int cont_centipede_exists(int id)
{
	if (id < 0 && id >= CENTIPEDE_LIMIT)
		return 0;

	return centipede_data_array[id].used != 0;
}

int cont_centipedes_get_count()
{
	int c = 0;

	for (int i = 0; i < CENTIPEDE_LIMIT; i++) {
		if (cont_centipede_exists(i)) {
			c++;
		}
	}

	return c;
}

int cont_centipedes_get_limit()
{
	return CENTIPEDE_LIMIT;
}

void cont_centipede_destroy(int id)
{
	if (!cont_centipede_exists(id))
		return;

	cont_timer_destroy(centipede_data_array[id].moveTimer);
	centipede_data_array[id].used = 0;
}

void cont_centipede_update(int id)
{
	if (!cont_centipede_exists(id))
		return;

	if (cont_timer_finished_reset(centipede_data_array[id].moveTimer)) {
		cont_centipede_move_h(id, centipede_data_array[id].mvDir ? 1 : -1);
	}

	if (centipede_data_array[id].length <= CENTIPEDE_DEATH_LENGTH) {
		cont_centipede_destroy(id);
	}
}

void cont_centipede_draw(int id)
{
	if(!cont_centipede_exists(id))
		return;

	attron(COLOR_PAIR(cont_colorpair_centipede));

	// draw the tail (stop if length is over limit)
	for (int i = 0; i < centipede_data_array[id].length && i < CENTIPEDE_LENGTH_LIMIT; i++) {
		mvprintw(centipede_data_array[id].prevY[i], centipede_data_array[id].prevX[i], "o");
	}

	// draw the head last
	mvprintw(centipede_data_array[id].y, centipede_data_array[id].x, "O");

	attroff(COLOR_PAIR(cont_colorpair_centipede));
}

void cont_centipedes_destroy()
{
	for (int i = 0; i < CENTIPEDE_LIMIT; i++) {
		cont_centipede_destroy(i);
	}
}

void cont_centipedes_update()
{
	for (int i = 0; i < CENTIPEDE_LIMIT; i++) {
		cont_centipede_update(i);
	}

	if (cont_centipedes_get_count() == 0) {
		int r = rand() % 2;
		int sx = 0;

		if (r)
			sx = getmaxx(stdscr) - 2;

		cont_centipede_create(0, 0, 1, centipedes_next_length, centipedes_next_length*1.5);
		centipedes_next_length *= 1.4;

		if (centipedes_next_length > CENTIPEDE_LENGTH_LIMIT) {
			centipedes_next_length = CENTIPEDE_LENGTH_LIMIT;
		}
	}
}

void cont_centipedes_draw()
{
	for (int i = 0; i < CENTIPEDE_LIMIT; i++) {
		cont_centipede_draw(i);
	}
}

int cont_centipede_get_move_dir(int id)
{
	if (!cont_centipede_exists(id))
		return 0;

	return centipede_data_array[id].mvDir;
}

void cont_centipede_set_move_dir(int id, int dir)
{
	if (!cont_centipede_exists(id))
		return;

	centipede_data_array[id].mvDir = dir;
}

int cont_centipede_get_x(int id)
{
	if (!cont_centipede_exists(id))
		return 0;

	return centipede_data_array[id].x;
}

int cont_centipede_get_y(int id)
{
	if (!cont_centipede_exists(id))
		return 0;

	return centipede_data_array[id].y;
}

void cont_centipede_set_x(int id, int x)
{
	if (!cont_centipede_exists(id))
		return;

	centipede_data_array[id].x = x;
}

void cont_centipede_set_y(int id, int y)
{
	if (!cont_centipede_exists(id))
		return;

	centipede_data_array[id].y = y;
}

void cont_centipede_set_yx(int id, int y, int x)
{
	if (!cont_centipede_exists(id))
		return;

	centipede_data_array[id].y = y;
	centipede_data_array[id].x = x;
}

void cont_centipede_move_h(int id, int h)
{
	if (!cont_centipede_exists(id))
		return;

	centipede_data_array[id].x += h;

	cont_centipede_push_tail(id);
}

void cont_centipede_move_v(int id, int v)
{
	if (!cont_centipede_exists(id))
		return;

	centipede_data_array[id].y += v;

	cont_centipede_push_tail(id);
}

void cont_centipede_move_vh(int id, int v, int h)
{
	if (!cont_centipede_exists(id))
		return;

	centipede_data_array[id].y += v;
	centipede_data_array[id].x += h;

	cont_centipede_push_tail(id);
}

void cont_centipede_split(int id, int at)
{
	char str[50];
	sprintf(str, "Centipede split at %i/%i, new length %i", at, centipede_data_array[id].length, centipede_data_array[id].length - at);
	//cont_debug(str);

	if (at >= cont_centipede_get_length(id))
		return;

	int newC = cont_centipede_create(centipede_data_array[id].prevY[at], centipede_data_array[id].prevX[at], !centipede_data_array[id].mvDir, centipede_data_array[id].length - at, centipede_data_array[id].basespeed);

	centipede_data_array[id].length -= at;

	cont_centipede_timer_reset(id);
}

int cont_centipede_get_tail_x(int id, int at)
{
	if (at >= cont_centipede_get_length(id))
		return 0;

	return centipede_data_array[id].prevX[at];
}

int cont_centipede_get_tail_y(int id, int at)
{
	if (at >= cont_centipede_get_length(id))
		return 0;

	return centipede_data_array[id].prevY[at];
}

int cont_centipede_get_length(int id)
{
	if (!cont_centipede_exists(id))
		return 0;

	return centipede_data_array[id].length;
}

int cont_centipedes_get_length_limit()
{
	return CENTIPEDE_LENGTH_LIMIT;
}

double cont_centipede_get_basespeed(int id)
{
	if (!cont_centipede_exists(id))
		return 0;

	return centipede_data_array[id].basespeed;
}

double cont_centipede_get_speed(int id)
{
	if (!cont_centipede_exists(id))
		return 0;

	return cont_centipede_speed(centipede_data_array[id].basespeed, centipede_data_array[id].length);
}

double cont_centipede_speed(double basespeed, int length)
{
	return basespeed / (length / 4);
}

void cont_centipedes_reset_next_length()
{
	centipedes_next_length = CENTIPEDE_START_LENGTH;
}

int cont_centipede_hit_debris(int id)
{
	if (!cont_centipede_exists(id))
		return 0;

	int x = centipede_data_array[id].x;
	int y = centipede_data_array[id].y;

	for (int i = 0; i < cont_alldebris_get_limit(); i++) {
		int xx = cont_debris_get_x(i);
		int yy = cont_debris_get_y(i);

		if (x == xx && y == yy) {
			return i;
		}
	}

	return -1;
}

int cont_centipede_hit_screenedge_x(int id)
{
	if (!cont_centipede_exists(id))
		return 0;

	if (centipede_data_array[id].x < 0) {
		return -centipede_data_array[id].x;
	}
	else if (centipede_data_array[id].x >= (getmaxx(stdscr) - 1)) {
		return -(centipede_data_array[id].x - (getmaxx(stdscr) - 1));
	}

	return 0;
}

void cont_centipede_push_tail(int id)
{
	if (!cont_centipede_exists(id))
		return;

	int xO = cont_centipede_hit_screenedge_x(id);

	if (xO != 0) {
		centipede_data_array[id].x += xO;
		centipede_data_array[id].y += 1;
		centipede_data_array[id].mvDir = !centipede_data_array[id].mvDir;
	}

	int d = cont_centipede_hit_debris(id);

	if (d != -1) {
		centipede_data_array[id].y += 1;
		centipede_data_array[id].x += centipede_data_array[id].mvDir == 1 ? -1 : 1;
		centipede_data_array[id].mvDir = !centipede_data_array[id].mvDir;
	}

	for (int t = CENTIPEDE_LENGTH_LIMIT-1; t > 0; t--) {
		centipede_data_array[id].prevX[t] = centipede_data_array[id].prevX[t - 1];
		centipede_data_array[id].prevY[t] = centipede_data_array[id].prevY[t - 1];
	}

	centipede_data_array[id].prevX[0] = centipede_data_array[id].x;
	centipede_data_array[id].prevY[0] = centipede_data_array[id].y;
}

void cont_centipede_set_length(int id, int length)
{
	if (!cont_centipede_exists(id))
		return;

	centipede_data_array[id].length = length;

	cont_centipede_timer_reset(id);
}

void cont_centipede_set_basespeed(int id, double basespeed)
{
	if (!cont_centipede_exists(id))
		return;

	centipede_data_array[id].basespeed = basespeed;

	cont_centipede_timer_reset(id);
}

void cont_centipede_timer_reset(int id)
{
	if (!cont_centipede_exists(id))
		return;

	cont_timer_set(centipede_data_array[id].moveTimer, cont_plat_timeout_from_speed(cont_centipede_get_speed(id)));
}
