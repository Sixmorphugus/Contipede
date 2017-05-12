#pragma once

// The centipede enemy. Moves left or right, moving down and changing direction every time they hit a wall.
// The centipede has a max length and records its previous positions for its tail to be.
// Headshotting a centipede will instakill it.
// Shooting the body will cause it to split into two centipedes which go in different directions.

// centipede management
void cont_centipedes_init();

int cont_centipede_create(int y, int x, int movedir, int length, double basespeed);
int cont_centipede_exists(int id);
int cont_centipedes_get_count();
int cont_centipedes_get_limit();

void cont_centipede_destroy(int id);
void cont_centipede_update(int id);
void cont_centipede_draw(int id);

void cont_centipedes_destroy();
void cont_centipedes_update();
void cont_centipedes_draw();

// positioning
int cont_centipede_get_move_dir(int id);
void cont_centipede_set_move_dir(int id, int dir);

int cont_centipede_get_x(int id);
int cont_centipede_get_y(int id);

void cont_centipede_set_x(int id, int x);
void cont_centipede_set_y(int id, int y);
void cont_centipede_set_yx(int id, int y, int x);

// note: moving the centipede with these functions will push to the previous position list
void cont_centipede_move_h(int id, int h);
void cont_centipede_move_v(int id, int v);
void cont_centipede_move_vh(int id, int v, int h);

// tail
void cont_centipede_split(int id, int at);
void cont_centipede_tail_x(int id, int at);
void cont_centipede_tail_y(int id, int at);

void cont_centipede_push_tail(int id);

void cont_centipede_set_length(int id, int length);
void cont_centipede_set_basespeed(int id, double basespeed);

int cont_centipede_get_length(int id);
int cont_centipedes_get_length_limit();
double cont_centipede_get_basespeed(int id);
double cont_centipede_get_speed(int id);

double cont_centipede_speed(int basespeed, int length);

// hit
int cont_centipede_hit_debris(int id);
int cont_centipede_hit_ship(int id);