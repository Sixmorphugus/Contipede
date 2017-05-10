// INCLUDES (we need PI)
#define _USE_MATH_DEFINES

#include <math.h>
#include <stdlib.h>
#include "curses.h"

#include "contipede_debris.h"
#include "contipede_debug.h"
#include "contipede_bullet.h"
#include "contipede_colorpairs.h"
#include "contipede_ship.h"

// DEFINITIONS
#define DEBRIS_LIMIT 80
#define DEBRIS_DMGCOLOR0 COLOR_WHITE
#define DEBRIS_DMGCOLOR1 COLOR_BLUE
#define DEBRIS_DMGCOLOR2 COLOR_MAGENTA
#define DEBRIS_DMGCOLOR3 COLOR_RED

typedef struct
{
	int x, y;
	int health, maxhealth;
	int destructiveness;
	char icon;
	int used;
} debris_data;

debris_data debris_data_array[DEBRIS_LIMIT];

void cont_alldebris_init()
{
	init_pair(cont_colorpair_debris_dmgnone, DEBRIS_DMGCOLOR0, COLOR_BLACK);
	init_pair(cont_colorpair_debris_dmg1, DEBRIS_DMGCOLOR1, COLOR_BLACK);
	init_pair(cont_colorpair_debris_dmg2, DEBRIS_DMGCOLOR2, COLOR_BLACK);
	init_pair(cont_colorpair_debris_dmg3, DEBRIS_DMGCOLOR3, COLOR_BLACK);

	for (int i = 0; i < DEBRIS_LIMIT; i++) {
		debris_data_array[i].used = 0;
	}
}

void cont_alldebris_generaterandom(int seed, int count)
{
	char str[50];
	sprintf(str, "Generating %i bits of debris (seed %i)", count, seed);
	cont_debug(str);

	srand(seed);

	for (int i = 0; i < count; i++) {
		// generate position randomly
		int x = rand() % getmaxx(stdscr);
		int y = rand() % getmaxy(stdscr);

		if (y >= cont_ship_get_y() - 5) {
			y = cont_ship_get_y() - 6;
		}

		// generate random icon (of 3 possible)
		int ic = rand() % 3;
		char ico;

		switch (ic) {
		default:
			ico = '@';
			break;
		case 1:
			ico = '#';
			break;
		case 2:
			ico = '+';
			break;
		}

		// random max health
		int h = 3 + (rand() % 3);

		// is it explosive?
		int d = 0;

		if (ic == 1) {
			d = 16;
		}
		else if (ic == 2) {
			d = 4;
		}

		cont_debris_create(ico, h, d, y, x);
	}

	cont_debug("Finished.");
}

int cont_debris_create(char icon, int health, int destructiveness, int y, int x)
{
	for (int i = 0; i < DEBRIS_LIMIT; i++) {
		if (!debris_data_array[i].used) {
			debris_data_array[i].icon = icon;
			debris_data_array[i].health = health;
			debris_data_array[i].maxhealth = health;
			debris_data_array[i].x = x;
			debris_data_array[i].y = y;
			debris_data_array[i].destructiveness = destructiveness;

			debris_data_array[i].used = 1;

			return i;
		}
	}

	cont_debug("Failed to create a new debris - debris limit hit");
	return -1;
}

int cont_debris_exists(int id)
{
	if (id < 0 || id >= DEBRIS_LIMIT)
		return 0;

	return debris_data_array[id].used;
}

int cont_debris_get_x(int id)
{
	if(!cont_debris_exists(id))
		return 0;

	return debris_data_array[id].x;
}

int cont_debris_get_y(int id)
{
	if (!cont_debris_exists(id))
		return 0;

	return debris_data_array[id].y;
}

void cont_debris_set_x(int id, int x)
{
	if (!cont_debris_exists(id))
		return;

	debris_data_array[id].x = x;
}

void cont_debris_set_y(int id, int y)
{
	if (!cont_debris_exists(id))
		return;

	debris_data_array[id].y = y;
}

void cont_debris_set_yx(int id, int y, int x)
{
	if (!cont_debris_exists(id))
		return;

	debris_data_array[id].y = y;
	debris_data_array[id].x = x;
}

int cont_debris_get_health(int id)
{
	if (!cont_debris_exists(id))
		return 0;

	return debris_data_array[id].health;
}

int cont_debris_get_maxhealth(int id)
{
	if (!cont_debris_exists(id))
		return 0;

	return debris_data_array[id].maxhealth;
}

int cont_debris_get_destructiveness(int id)
{
	if (!cont_debris_exists(id))
		return 0;

	return debris_data_array[id].destructiveness;
}

int cont_debris_get_relative_displaycolor(int id)
{
	if (!cont_debris_exists(id))
		return cont_colorpair_debris_dmgnone;

	 // divide health by max health
	double hM = (double)debris_data_array[id].health / (double)debris_data_array[id].maxhealth;

	if (hM == 1) {
		return cont_colorpair_debris_dmgnone;
	}
	else if (hM >= 0.66) {
		return cont_colorpair_debris_dmg1;
	}
	else if (hM >= 0.33) {
		return cont_colorpair_debris_dmg2;
	}
	
	return cont_colorpair_debris_dmg3;
}

void cont_debris_set_health(int id, int h)
{
	if (!cont_debris_exists(id))
		return;

	debris_data_array[id].health = h;
}

void cont_debris_set_maxhealth(int id, int h)
{
	if (!cont_debris_exists(id))
		return;

	debris_data_array[id].maxhealth = h;
}

void cont_debris_set_destructiveness(int id, int destructiveness)
{
	if (!cont_debris_exists(id))
		return;

	debris_data_array[id].destructiveness = destructiveness;
}

void cont_debris_inc_health(int id, int health)
{
	if (!cont_debris_exists(id))
		return;

	debris_data_array[id].health += health;
}

int cont_alldebris_get_count()
{
	int c = 0;

	for (int i = 0; i < DEBRIS_LIMIT; i++) {
		if (cont_debris_exists(i)) {
			c++;
		}
	}

	return c;
}

int cont_alldebris_get_limit()
{
	return DEBRIS_LIMIT;
}

void cont_debris_destroy(int id)
{
	if (!cont_debris_exists(id))
		return;

	debris_data_array[id].used = 0;
}

void cont_debris_update(int id)
{
	if (!cont_debris_exists(id))
		return;

	if (debris_data_array[id].health > debris_data_array[id].maxhealth) { // don't exceed the hard limit
		debris_data_array[id].health = debris_data_array[id].maxhealth;
	}

	if (debris_data_array[id].health <= 0) {
		// debris was destroyed
		int d = debris_data_array[id].destructiveness;

		int x = debris_data_array[id].x;
		int y = debris_data_array[id].y;

		if (d > 0) {
			// spawn projectiles in circular motion
			double circBlit = (M_PI * 2.0) / (double)d;
			for (int i = 0; i < d; i++) {
				// find the relative angle at which this debris is fired outwards
				double thisCircBlit = circBlit * i;

				// convert said angle to a direction
				double xSpd = (int)(cos(thisCircBlit) * 10);
				double ySpd = (int)(sin(thisCircBlit) * 10);

				// create bullet
				cont_bullet_create('\'', 0, y, x, ySpd, xSpd);
			}
		}

		cont_debris_destroy(id);
	}
}

void cont_debris_draw(int id)
{
	int cp = cont_debris_get_relative_displaycolor(id); // this has its own "exists" check :/

	if (!cont_debris_exists(id))
		return;

	// again we assemble a two character nullterminated string for our icon
	char istr[2];
	istr[0] = debris_data_array[id].icon;
	istr[1] = '\0';

	attron(COLOR_PAIR(cp));
	mvprintw(debris_data_array[id].y, debris_data_array[id].x, istr);
	attroff(COLOR_PAIR(cp));
}

void cont_alldebris_destroy()
{
	for (int i = 0; i < DEBRIS_LIMIT; i++) {
		cont_debris_destroy(i);
	}
}

void cont_alldebris_update()
{
	for (int i = 0; i < DEBRIS_LIMIT; i++) {
		cont_debris_update(i);
	}
}

void cont_alldebris_draw()
{
	for (int i = 0; i < DEBRIS_LIMIT; i++) {
		cont_debris_draw(i);
	}
}
