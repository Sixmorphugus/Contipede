#include "contipede_timer.h"
#include "contipede_platform.h"
#include "contipede_debug.h"

#include <stdio.h>

// data
#define TIMER_LIMIT 500

typedef struct {
	int remaining; // remaining time on timer
	int lset; // last set time (for timer_reset)
	int used;
} timer_data;

timer_data timer_data_array[TIMER_LIMIT];

int cont_timer_finished(int id)
{
	return cont_timer_remaining(id) <= 0;
}

int cont_timer_remaining(int id)
{
	if (!cont_timer_exists(id))
		return 0;

	return timer_data_array[id].remaining;
}

int cont_timer_reset(int id)
{
	if (!cont_timer_exists(id))
		return 0;

	int r = timer_data_array[id].remaining;

	timer_data_array[id].remaining = timer_data_array[id].lset;

	return r;
}

int cont_timer_finished_reset(int id)
{
	int f = cont_timer_finished(id);

	if (f)
		cont_timer_reset(id);

	return f;
}

int cont_timer_set(int id, int ms)
{
	if (!cont_timer_exists(id))
		return 0;

	int r = cont_timer_remaining(id);

	timer_data_array[id].lset = ms;
	timer_data_array[id].remaining = ms;

	return r;
}

int cont_timer_create(int ms)
{
	for (unsigned int i = 0; i < TIMER_LIMIT; i++) {
		if (!cont_timer_exists(i)) {
			timer_data_array[i].remaining = ms;
			timer_data_array[i].lset = ms;
			
			timer_data_array[i].used = 1;

			return i;
		}
	}

	cont_debug("Failed to create timer - limit hit");
	return -1;
}

int cont_timer_exists(int id)
{
	if (id < 0 || id >= TIMER_LIMIT)
		return 0;
	
	return timer_data_array[id].used != 0;
}

void cont_timer_destroy(int id)
{
	if (!cont_timer_exists(id))
		return;

	timer_data_array[id].used = 0;
}

void cont_timer_update(int id)
{
	if (cont_timer_exists(id)) {
		if (timer_data_array[id].remaining > 0) {
			timer_data_array[id].remaining -= cont_plat_get_frametime();

			/*
			char st[50];
			sprintf(st, "%i remaining", timer_data_array[id].remaining);
			cont_debug(st);
			*/
		}
	}
}

void cont_timers_init()
{
	for (int i = 0; i < TIMER_LIMIT; i++) {
		timer_data_array[i].used = 0;
	}
}

void cont_timers_update()
{
	for (int i = 0; i < TIMER_LIMIT; i++) {
		cont_timer_update(i);
	}
}

int cont_timers_get_count()
{
	int c = 0;

	for (int i = 0; i < TIMER_LIMIT; i++) {
		if (timer_data_array[i].used)
			c++; // a better language
	}

	return c;
}

int cont_timers_get_limit()
{
	return TIMER_LIMIT;
}

