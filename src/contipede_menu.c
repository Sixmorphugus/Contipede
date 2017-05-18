#include "curses.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "contipede_menu.h"
#include "contipede_colorpairs.h"
#include "contipede_ship.h"
#include "contipede_debris.h"
#include "contipede_timer.h"
#include "contipede_centipede.h"
#include "contipede_bullet.h"

#define MENU_BGCOLOR COLOR_YELLOW
#define MENU_NUM_SCORES 9

#define _CRT_SECURE_NO_WARNINGS

int menu_selected;
int menu_enabled;
int menu_quit;
int menu_appeartimer;
int menu_highscores[MENU_NUM_SCORES];

void cont_menu_init()
{
	menu_selected = 0;
	menu_enabled = 1;
	menu_quit = 0;
	menu_appeartimer = -1;

	for (int i = 0; i < MENU_NUM_SCORES; i++) {
		menu_highscores[i] = 0;
	}

	init_pair(cont_colorpair_menuback, COLOR_WHITE, MENU_BGCOLOR);

	// load scorelist
	cont_menu_loadscores();
}

void cont_menu_update()
{
	if (menu_appeartimer != -1) {
		if (cont_timer_finished(menu_appeartimer)) {
			menu_enabled = 1;
			cont_timer_destroy(menu_appeartimer);
			menu_appeartimer = -1;
		}
	}

	if (!menu_enabled)
		return;

	if (menu_selected > 1)
		menu_selected = 0;
	else if (menu_selected < 0) {
		menu_selected = 1;
	}
}

void cont_menu_draw()
{
	if (!menu_enabled)
		return;

	char title[] = " C O N T I P E D E ! ";

	attron(COLOR_PAIR(cont_colorpair_menuback));
	mvprintw(5, (getmaxx(stdscr) / 2) - (strlen(title) / 2), title);
	attroff(COLOR_PAIR(cont_colorpair_menuback));

	for (int i = 0; i < MENU_NUM_SCORES; i++) {
		char sc[20];

		int ui_score = menu_highscores[i];

		if (ui_score < 10)
			sprintf(sc, "00000%i", ui_score);
		else if (ui_score < 100)
			sprintf(sc, "0000%i", ui_score);
		else if (ui_score < 1000)
			sprintf(sc, "000%i", ui_score);
		else if (ui_score < 10000)
			sprintf(sc, "00%i", ui_score);
		else if (ui_score < 100000)
			sprintf(sc, "0%i", ui_score);
		else
			sprintf(sc, "%i", ui_score);

		mvprintw(8+i, (getmaxx(stdscr) / 2) - (strlen(sc) / 2), sc);
	}

	char op1[] = "begin game";
	char op1s[] = "[ BEGIN GAME ]";

	char op2[] = "exit";
	char op2s[] = "[ EXIT ]";

	mvprintw(8 + MENU_NUM_SCORES + 2, (getmaxx(stdscr) / 2) - (strlen(menu_selected == 0 ? op1s : op1) / 2), menu_selected == 0 ? op1s : op1);
	mvprintw(8 + MENU_NUM_SCORES + 4, (getmaxx(stdscr) / 2) - (strlen(menu_selected == 1 ? op2s : op2) / 2), menu_selected == 1 ? op2s : op2);
}

void cont_menu_sendch(int ch)
{
	if (!menu_enabled)
		return;

	if (ch == KEY_UP) {
		menu_selected--;
	}
	else if (ch == KEY_DOWN) {
		menu_selected++;
	}
	else if (ch == 10) {
		cont_menu_choose_option();
	}
}

void cont_menu_set_option(int op)
{
	menu_selected = op;
}

int cont_menu_get_option()
{
	return menu_selected;
}

void cont_menu_savescores()
{
	FILE* scorelist = fopen("scorelist.txt", "w");
	
	if (scorelist == NULL)
	{
		printf("Error opening scorelist.txt!\n");
		return;
	}

	for (int i = 0; i < MENU_NUM_SCORES; i++) {
		fprintf(scorelist, "%i\n", menu_highscores[i]);
	}

	fclose(scorelist);
}

void cont_menu_loadscores()
{
	FILE* scorelist = fopen("scorelist.txt", "r");

	if (scorelist == NULL)
	{
		printf("Error opening scorelist.txt!\n");
		return;
	}

	char line[8];
	int score;

	while (fgets(line, 80, scorelist) != NULL)
	{
		/* get a line, up to 80 chars from fr.  done if NULL */
		sscanf(line, "%i", &score);
		cont_menu_push_score(score);
	}

	fclose(scorelist);
}

void cont_menu_choose_option()
{
	if (menu_selected == 0) {
		cont_ship_set_state(ship_state_NORMAL);

		cont_alldebris_destroy();
		cont_centipedes_destroy();
		cont_bullets_destroy();

		cont_alldebris_generaterandom(time(0), cont_alldebris_get_limit());
	}
	else if (menu_selected == 1) {
		menu_quit = 1;
		cont_menu_savescores();
	}

	menu_enabled = 0;
}

void cont_menu_set_enabled(int enabled)
{
	menu_enabled = enabled;
}

int cont_menu_get_enabled()
{
	return menu_enabled;
}

int cont_menu_get_quit()
{
	return menu_quit;
}

void cont_menu_push_score(int hs)
{
	for (int i = 0; i < MENU_NUM_SCORES; i++) {
		if (menu_highscores[i] < hs) {
			// go down the table replacing scores
			for (int t = MENU_NUM_SCORES - 1; t > i; t--) {
				menu_highscores[t] = menu_highscores[t - 1];
				menu_highscores[t] = menu_highscores[t - 1];
			}

			menu_highscores[i] = hs;
			menu_highscores[i] = hs;

			break;
		}
	}
}

int cont_menu_get_score(int id)
{
	if (id < 0 || id > MENU_NUM_SCORES)
		return 0;

	return menu_highscores[id];
}

void cont_menu_get_score_count()
{
	return MENU_NUM_SCORES;
}

void cont_menu_appear_after(int ms)
{
	menu_appeartimer = cont_timer_create(ms);
	cont_menu_push_score(cont_ui_get_score());
	cont_ui_set_score(0);
}
