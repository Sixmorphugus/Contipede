#pragma once
// Timers are settable clocks that run in the background.
// You can allocate them and destroy them for many specific purposes.

// timing
int cont_timer_finished(int id); // returns whether timer reached <= 0
int cont_timer_remaining(int id); // returns remaining time
int cont_timer_reset(int id); // resets timer to last set time, returns remaining time
int cont_timer_finished_reset(int id); // resets the timer if it finished - returns whether it finished
int cont_timer_set(int id, int ms); // sets new time on timer, returns previous one

// management
int cont_timer_create(int ms);

int cont_timer_exists(int id);

void cont_timer_destroy(int id);
void cont_timer_update(int id);

void cont_timers_init();
void cont_timers_update();

int cont_timers_get_count();
int cont_timers_get_limit();