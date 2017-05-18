#include "contipede_ui.h"
#include "contipede_ship.h"
#include "contipede_colorpairs.h"

#include <curses.h>

int ui_score;
#define SCORE_COLOR COLOR_GREEN

void cont_ui_init_score()
{
	ui_score = 0;
	init_pair(cont_colorpair_score, COLOR_WHITE, SCORE_COLOR);
}

void cont_ui_inc_score(int score)
{
	ui_score += score;
}

void cont_ui_set_score(int score)
{
	ui_score = score;
}

int cont_ui_get_score()
{
	return ui_score;
}

void cont_ui_draw_score()
{
	if (cont_ship_get_state() != ship_state_DEAD) {
		char sc[20];

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

		attron(COLOR_PAIR(cont_colorpair_score));
		mvprintw(1, (getmaxx(stdscr) / 2) - (strlen(sc) / 2), sc);
		attroff(COLOR_PAIR(cont_colorpair_score));
	}
}
