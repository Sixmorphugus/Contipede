#include "curses.h"

#include "contipede_centipede.h"
#include "contipede_colorpairs.h"

// some constants!
#define CENTIPEDE_LIMIT 100
#define CENTIPEDE_LENGTH_LIMIT 100

#define CENTIPEDE_COLOR COLOR_GREEN

typedef struct {
	int used;

	int x, y;
	double basespeed;
	int mvDir;
	int length;

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
