#pragma once

// debris positioning
int cont_debris_get_x(int id);
int cont_debris_get_y(int id);

void cont_debris_set_x(int id, int x);
void cont_debris_set_y(int id, int y);
void cont_debris_set_yx(int id, int y, int x);

// debris health
int cont_debris_get_health(int id);
int cont_debris_get_maxhealth(int id);
int cont_debris_get_destructiveness(int id);

int cont_debris_get_relative_displaycolor(int id);

void cont_debris_set_health(int id, int health);
void cont_debris_set_maxhealth(int id, int health);
void cont_debris_set_destructiveness(int id, int destructiveness);

void cont_debris_inc_health(int id, int health);

// debris management
void cont_alldebris_init();
void cont_alldebris_generaterandom(int seed, int count);

int cont_debris_create(char icon, int health, int destructiveness, int y, int x);
int cont_debris_exists(int id);

int cont_alldebris_get_count();
int cont_alldebris_get_limit();

void cont_debris_destroy(int id);
void cont_debris_update(int id);
void cont_debris_draw(int id);

void cont_alldebris_destroy();
void cont_alldebris_update();
void cont_alldebris_draw();
