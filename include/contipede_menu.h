#pragma once
// Main menu stuff.

void cont_menu_init();

void cont_menu_update();
void cont_menu_draw();
void cont_menu_sendch(int ch);

void cont_menu_set_option(int op);
int cont_menu_get_option();

void cont_menu_savescores();
void cont_menu_loadscores();

void cont_menu_choose_option();

void cont_menu_set_enabled(int enabled);
int cont_menu_get_enabled();

int cont_menu_get_quit();

void cont_menu_push_score(int hs);
int cont_menu_get_score(int id);
void cont_menu_get_score_count();

void cont_menu_appear_after(int ms);