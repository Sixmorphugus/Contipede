#include "curses.h"
#include <string.h>
#include <time.h>

#include "contipede_menu.h"
#include "contipede_colorpairs.h"
#include "contipede_ship.h"
#include "contipede_debris.h"
#include "contipede_timer.h"

#define MENU_BGCOLOR COLOR_YELLOW

int menu_selected;
int menu_enabled;
int menu_quit;
int menu_appeartimer;

void cont_menu_init()
{
	menu_selected = 0;
	menu_enabled = 1;
	menu_quit = 0;
	menu_appeartimer = -1;

	init_pair(cont_colorpair_menuback, COLOR_WHITE, MENU_BGCOLOR);
}

void cont_menu_update()
{
	if (!menu_enabled)
		return;

	if (menu_selected > 1)
		menu_selected = 0;
	else if (menu_selected < 0) {
		menu_selected = 1;
	}

	if (menu_appeartimer != -1) {
		if (cont_timer_finished(menu_appeartimer)) {
			menu_enabled = 1;
			cont_timer_destroy(menu_appeartimer);
			menu_appeartimer = -1;
		}
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

	char op1[] = "begin game";
	char op1s[] = "[ BEGIN GAME ]";

	char op2[] = "exit";
	char op2s[] = "[ EXIT ]";

	mvprintw(10, (getmaxx(stdscr) / 2) - (strlen(menu_selected == 0 ? op1s : op1) / 2), menu_selected == 0 ? op1s : op1);
	mvprintw(12, (getmaxx(stdscr) / 2) - (strlen(menu_selected == 1 ? op2s : op2) / 2), menu_selected == 1 ? op2s : op2);
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
	}

	menu_enabled = 0;
}

int cont_menu_get_quit()
{
	return menu_quit;
}

void cont_menu_appear_after(int ms)
{
	menu_appeartimer = cont_timer_create(ms);
}
