#pragma once

// contipede_ship houses all the functionality for the ship
// it stores its variables as global but they are isolated in the C file
// because it's designed this way, Contipede can only support one ship at once.

// Note how y and x are that way around; this mimics the order curses has them

enum
{
	ship_state_NORMAL,
	ship_state_FROZEN,
	ship_state_DEAD
};

// processors
void cont_ship_init(); // init_reset the ship

void cont_ship_sendch(int ch); // respond to input (moving, firing etc)
void cont_ship_update(); // check if the ship can be where it is, 

// Ship Positioning Functions
void cont_ship_setx(int x);
void cont_ship_sety(int y);
void cont_ship_setyx(int y, int x);

void cont_ship_moveh(int x);
void cont_ship_movev(int y);
void cont_ship_movevh(int v, int h);

int cont_ship_getx();
int cont_ship_gety();

// ship state
int ship_state_get();
void ship_state_set(int st);

// drawing
void cont_ship_draw(); // draw the ship with curses. 

// other
void cont_ship_fire(); // creates a projectile above the ship