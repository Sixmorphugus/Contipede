#include "contipede_timer.h"
#include "contipede_platform.h"

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
	if (cont_timer_exists(id))
		return 0;

	return timer_data_array[id].remaining <= 0;
}

int cont_timer_reset(int id)
{
	if (cont_timer_exists(id))
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

