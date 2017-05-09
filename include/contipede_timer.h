#pragma once
// Timers are settable clocks that run in the background.
// You can allocate them and destroy them for many specific purposes.

// timing
int cont_timer_finished(int id);
int cont_timer_remaining(int id);
int cont_timer_set(int id, int ms);

// management
int cont_timer_create(int ms);
void cont_timer_destroy(int id);

void cont_timer_update(int id);

void cont_timers_update();
void cont_timers_get_count();
void cont_timers_get_limit();