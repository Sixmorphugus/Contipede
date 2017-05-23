#pragma once

// there can only be one moth onscreen at once, so moths are not array managed.
// moths shoot at you and you can get points for hitting them, however them reaching you will not kill you.
// They move in a horizontal sine wave pattern.

enum
{
	moth_state_NORMAL,
	moth_state_DEAD
};

// processors
void cont_moth_init(); // init/reset the moth system

void cont_moth_update(); // move the moth

// Moth Positioning Functions
void cont_moth_set_x(int x);
void cont_moth_set_y(int y);
void cont_moth_set_yx(int y, int x);

void cont_moth_move_h(int x);
void cont_moth_move_v(int y);
void cont_moth_move_vh(int v, int h);

int cont_moth_get_x();
int cont_moth_get_y();

// moth state
int cont_moth_get_state();
void cont_moth_set_state(int st);

// drawing
void cont_moth_draw(); // draw the moth with curses. 

					   // other
void cont_moth_fire(); // creates a projectile below the moth