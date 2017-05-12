#include "curses.h"

#include "contipede_centipede.h"
#include "contipede_colorpairs.h"
#include "contipede_timer.h"
#include "contipede_platform.h"
#include "contipede_debug.h"

// some constants!
#define CENTIPEDE_LIMIT 100
#define CENTIPEDE_LENGTH_LIMIT 10

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

centipede_data centipede_data_array[CENTIPEDE_LIMIT];

void cont_centipedes_init()
{
	init_pair(cont_colorpair_centipede, CENTIPEDE_COLOR, COLOR_BLACK);

	for (int i = 0; i < CENTIPEDE_LIMIT; i++) {
		centipede_data_array[i].used = 0;
	}
}

int cont_centipede_create(int y, int x, int movedir, int length, double basespeed)
{
	for (int i = 0; i < CENTIPEDE_LIMIT; i++) {
		if (!cont_centipede_exists(i)) {
			if (length > CENTIPEDE_LENGTH_LIMIT) {
				cont_debug("Given centipede length is illegal - trimmed");
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

	centipede_data_array[id].used = 0;
}

void cont_centipede_update(int id)
{
	if (!cont_centipede_exists(id))
		return;

	if (cont_timer_finished_reset(centipede_data_array[id].moveTimer)) {
		cont_centipede_move_h(id, centipede_data_array[id].mvDir ? 1 : -1);
	}
}

void cont_centipede_draw(int id)
{
	if(!cont_centipede_exists(id))
		return;

	attron(COLOR_PAIR(cont_colorpair_centipede));

	// draw the tail (stop if length is over limit)
	for (int i = 0; i < CENTIPEDE_LENGTH_LIMIT; i++) {
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
	if (at >= cont_centipede_get_length(id))
		return;

	int newC = cont_centipede_create(centipede_data_array[id].prevY[at], centipede_data_array[id].prevX[at], !centipede_data_array[id].mvDir, centipede_data_array[id].length - at, centipede_data_array[id].basespeed);

	centipede_data_array[id].length = at-1;
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
	return basespeed / length;
}

void cont_centipede_push_tail(int id)
{
	if (!cont_centipede_exists(id))
		return;

	for (int t = 1; t < CENTIPEDE_LENGTH_LIMIT; t++) {
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
