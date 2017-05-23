// DEFS
#define BULLET_LIMIT 1000
#define BULLET_COLOR COLOR_CYAN

// included files
#include "curses.h"
#include <math.h>

#include "contipede_bullet.h"
#include "contipede_ship.h" // for ship collision
#include "contipede_debris.h" // for debris collision
#include "contipede_debug.h"
#include "contipede_colorpairs.h"
#include "contipede_timer.h"
#include "contipede_platform.h"
#include "contipede_centipede.h"
#include "contipede_menu.h"
#include "contipede_ui.h"
#include "contipede_moth.h"

// bullet data structure and storage
typedef struct {
	char icon;
	double hSpeed, vSpeed;
	int hTimer, vTimer;
	int x, y;
	int friendlyBullet;
	int used;
} bullet_data;

bullet_data bullet_data_array[BULLET_LIMIT];

int cont_bullet_get_x(int id)
{
	if (!cont_bullet_exists(id))
		return 0;

	return bullet_data_array[id].x;
}

int cont_bullet_get_y(int id)
{
	if (!cont_bullet_exists(id))
		return 0;

	return bullet_data_array[id].y;
}

void cont_bullet_set_x(int id, int x)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].x = x;
}

void cont_bullet_set_y(int id, int y)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].y = y;
}

void cont_bullet_set_yx(int id, int y, int x)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].y = y;
	bullet_data_array[id].x = x;
}

void cont_bullet_moveh(int id, int h)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].x += h;
}

void cont_bullet_movev(int id, int v)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].y += v;
}

void cont_bullet_movevh(int id, int v, int h)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].y += v;
	bullet_data_array[id].x += h;
}

double cont_bullet_get_hspeed(int id)
{
	if (!cont_bullet_exists(id))
		return 0;

	return bullet_data_array[id].hSpeed;
}

double cont_bullet_get_vspeed(int id)
{
	if (!cont_bullet_exists(id))
		return 0;

	return bullet_data_array[id].vSpeed;
}

void cont_bullet_set_hspeed(int id, double h)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].hSpeed = cont_plat_speed_increment(h);
	cont_timer_set(bullet_data_array[id].hTimer, cont_plat_timeout_from_speed(h));
}

void cont_bullet_set_vspeed(int id, double v)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].vSpeed = cont_plat_speed_increment(v);
	cont_timer_set(bullet_data_array[id].vTimer, cont_plat_timeout_from_speed(v));
}

void cont_bullet_set_vhspeed(int id, double v, double h)
{
	cont_bullet_set_vspeed(id, v);
	cont_bullet_set_hspeed(id, h);
}

void cont_bullets_init()
{
	for (unsigned int i = 0; i < BULLET_LIMIT; i++) {
		bullet_data_array[i].used = 0;
	}

	init_pair(cont_colorpair_bullet, BULLET_COLOR, COLOR_BLACK);
}

int cont_bullet_create(char icon, int friendly, int y, int x, double vS, double hS)
{
	// find first empty bullet slot
	for (unsigned int i = 0; i < BULLET_LIMIT; i++) {
		if (!cont_bullet_exists(i)) {
			bullet_data* b = &bullet_data_array[i];

			b->icon = icon;
			b->friendlyBullet = friendly;
			b->x = x;
			b->y = y;
			b->hSpeed = cont_plat_speed_increment(hS);
			b->vSpeed = cont_plat_speed_increment(vS);
			b->hTimer = cont_timer_create(cont_plat_timeout_from_speed(hS));
			b->vTimer = cont_timer_create(cont_plat_timeout_from_speed(vS));

			if (b->hTimer == -1 || b->vTimer == -1) {
				cont_debug("Failed to create a new bullet - timer limit hit");
				cont_timer_destroy(b->hTimer);
				return -1;
			}

			b->used = 1;

			return i;
		}
	}

	cont_debug("Failed to create a new bullet - limit hit");
	return -1; // unable to create new bullet because too many already exist
}

int cont_bullet_exists(int id)
{
	if(id < 0 || id >= BULLET_LIMIT)
		return 0;

	return bullet_data_array[id].used > 0;
}

int cont_bullets_get_count()
{
	int num = 0;

	for (unsigned int i = 0; i < BULLET_LIMIT; i++) {
		if (!bullet_data_array[i].used) {
			num++;
		}
	}

	return num;
}

int cont_bullets_get_limit()
{
	return BULLET_LIMIT;
}

void cont_bullet_destroy(int id)
{
	if (!cont_bullet_exists(id))
		return;

	// free timers before declaring this bullet to be "free"
	cont_timer_destroy(bullet_data_array[id].hTimer);
	cont_timer_destroy(bullet_data_array[id].vTimer);

	bullet_data_array[id].used = 0;
}

void cont_bullet_update(int id)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data* b = &bullet_data_array[id];

	// move bullet on
	if (cont_timer_finished_reset(b->hTimer))
		b->x += (int)b->hSpeed;
	if (cont_timer_finished_reset(b->vTimer)) {
		b->y += (int)b->vSpeed;
		//cont_debug("boop");
	}

	int s = cont_bullet_hit_screenedge(id);
	int d = cont_bullet_hit_debris(id);

	int ch = cont_bullet_hit_centipede(id);
	int ct = cont_bullet_hit_centipede_tail(id);

	int sh = cont_bullet_hit_ship(id);

	int mo = cont_bullet_hit_moth(id);

	if (s != -1)
		cont_bullet_destroy(id);
	else if (d != -1) {
		cont_bullet_destroy(id);
		cont_debris_inc_health(d, -1);
	}
	else if (ch != -1) {
		cont_ui_inc_score(cont_centipede_get_length(ch) * 2);

		cont_bullet_destroy(id);
		cont_centipede_destroy(ch);

		cont_debug("HEAD SHOT.");
	}
	else if (ct != -1) {
		// get the tail position the bullet actually hit
		int l = cont_centipede_get_length(ct);
		int at = l / 2;

		for (int i = 0; i < l; i++) {
			if (cont_centipede_get_tail_x(ct, i) == b->x && cont_centipede_get_tail_y(ct, i) == b->y) {
				at = i;
			}
		}

		cont_bullet_destroy(id);

		cont_centipede_split(ct, at);
	}
	else if (sh != -1 && !bullet_data_array[id].friendlyBullet) {
		cont_ship_set_state(ship_state_DEAD);
		cont_bullet_destroy(id);
		cont_centipedes_reset_next_length();

		cont_menu_appear_after(1000);
	}
	else if (mo != -1 && bullet_data_array[id].friendlyBullet) {
		cont_moth_set_state(moth_state_DEAD);
		cont_bullet_destroy(id);

		cont_ui_inc_score(100);
	}
}

void cont_bullet_draw(int id)
{
	if (!cont_bullet_exists(id))
		return;

	// turn our icon character into a string
	char str[2];
	str[0] = bullet_data_array[id].icon;
	str[1] = '\0';

	// draw it
	attron(COLOR_PAIR(cont_colorpair_bullet));
	mvprintw(bullet_data_array[id].y, bullet_data_array[id].x, str);
	attroff(COLOR_PAIR(cont_colorpair_bullet));
}

void cont_bullets_destroy()
{
	for (unsigned int i = 0; i < BULLET_LIMIT; i++) {
		bullet_data_array[i].used = 0;
	}
}

void cont_bullets_update()
{
	for (unsigned int i = 0; i < BULLET_LIMIT; i++) {
		cont_bullet_update(i);
	}
}

void cont_bullets_draw()
{
	for (unsigned int i = 0; i < BULLET_LIMIT; i++) {
		cont_bullet_draw(i);
	}
}

int cont_bullet_hit_screenedge(int id)
{
	if (!cont_bullet_exists(id))
		return -1;

	int x = bullet_data_array[id].x;
	int y = bullet_data_array[id].y;

	if (x < 0 || x >= getmaxx(stdscr) || y < 0 || y >= getmaxy(stdscr)) {
		return 1;
	}

	return -1;
}

int cont_bullet_hit_ship(int id)
{
	if (!cont_bullet_exists(id))
		return -1;

	int x = bullet_data_array[id].x;
	int y = bullet_data_array[id].y;

	int xx = cont_ship_get_x();
	int yy = cont_ship_get_y();

	if (y == yy && x >= xx - 1 && x <= xx + 1) {
		return 1;
	}

	return -1;
}

int cont_bullet_hit_moth(int id)
{
	if (!cont_bullet_exists(id))
		return -1;

	int x = bullet_data_array[id].x;
	int y = bullet_data_array[id].y;

	int xx = cont_moth_get_x();
	int yy = cont_moth_get_y();

	if (y == yy && x >= xx - 1 && x <= xx + 1) {
		return 1;
	}

	return -1;
}

int cont_bullet_hit_debris(int id)
{
	if (!cont_bullet_exists(id))
		return -1;

	int x = bullet_data_array[id].x;
	int y = bullet_data_array[id].y;

	for (int i = 0; i < cont_alldebris_get_limit(); i++) {
		if (!cont_debris_exists(i))
			continue;

		int xx = cont_debris_get_x(i);
		int yy = cont_debris_get_y(i);

		if (x == xx && y == yy) {
			return i;
		}
	}

	return -1;
}

int cont_bullet_hit_centipede(int id)
{
	if (!cont_bullet_exists(id))
		return -1;

	int x = bullet_data_array[id].x;
	int y = bullet_data_array[id].y;

	for (int i = 0; i < cont_centipedes_get_limit(); i++) {
		if (!cont_centipede_exists(i))
			continue;

		int xx = cont_centipede_get_x(i);
		int yy = cont_centipede_get_y(i);

		if (x == xx && y == yy) {
			return i;
		}
	}

	return -1;
}

int cont_bullet_hit_centipede_tail(int id)
{
	if (!cont_bullet_exists(id))
		return -1;

	int x = bullet_data_array[id].x;
	int y = bullet_data_array[id].y;

	for (int i = 0; i < cont_centipedes_get_limit(); i++) {
		if (!cont_centipede_exists(i))
			continue;

		for (int j = 0; j < cont_centipede_get_length(i); j++) {
			int xx = cont_centipede_get_tail_x(i, j);
			int yy = cont_centipede_get_tail_y(i, j);

			if (x == xx && y == yy) {
				return i;
			}
		}
	}

	return -1;
}

int cont_bullet_get_friendly(int id)
{
	if (!cont_bullet_exists(id))
		return 0;

	return bullet_data_array[id].friendlyBullet;
}

char cont_bullet_get_icon(int id)
{
	if (!cont_bullet_exists(id))
		return (char)0;

	return bullet_data_array[id].icon;
}

void cont_bullet_set_friendly(int id, int friendly)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].friendlyBullet = friendly;
}

void cont_bullet_set_icon(int id, char icon)
{
	if (!cont_bullet_exists(id))
		return;

	bullet_data_array[id].icon = icon;
}
