#pragma once

// contipede_bullet houses all the functionality for bullets
// it stores its variables as global but they are isolated in the C file
// multiple bullets can exist at once and have IDs and data

// Note how y and x are that way around; this mimics the order curses has them

// positioning
int cont_bullet_get_x(int id);
int cont_bullet_get_y(int id);

void cont_bullet_set_x(int id, int x);
void cont_bullet_set_y(int id, int y);
void cont_bullet_set_yx(int id, int y, int x);

void cont_bullet_moveh(int id, int v);
void cont_bullet_movev(int id, int v);
void cont_bullet_movevh(int id, int v, int h);

// speed
double cont_bullet_get_hspeed(int id);
double cont_bullet_get_vspeed(int id);

void cont_bullet_set_hspeed(int id, double h);
void cont_bullet_set_vspeed(int id, double v);
void cont_bullet_set_vhspeed(int id, double v, double h);

// other
int cont_bullet_get_friendly(int id);
char cont_bullet_get_icon(int id);

void cont_bullet_set_friendly(int id, int friendly);
void cont_bullet_set_icon(int id, char icon);

// bullet management
void cont_bullets_init();

int cont_bullet_create(char icon, int friendly, int y, int x, double vS, double hS);
int cont_bullet_exists(int id);
int cont_bullets_get_count();
int cont_bullets_get_limit();

void cont_bullet_destroy(int id);
void cont_bullet_update(int id);
void cont_bullet_draw(int id);

void cont_bullets_destroy();
void cont_bullets_update();
void cont_bullets_draw();

// bullet collision
int cont_bullet_hit_screenedge(int id);
int cont_bullet_hit_ship(int id);
int cont_bullet_hit_debris(int id);