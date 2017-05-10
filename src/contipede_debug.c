#include "curses.h"
#include <string.h>
#include "contipede_debug.h"
#include "contipede_colorpairs.h"
#include "contipede_timer.h"

#define DEBUG_LINES 15
#define DEBUG_TEXT_LENGTH 50
#define DEBUG_LINE_VIEWTIME 2000

char debug_lines[DEBUG_LINES][DEBUG_TEXT_LENGTH];
int debug_cur_line;
int debug_line_remove_timer;

void cont_debug_init()
{
	init_pair(cont_colorpair_debug, COLOR_BLACK, COLOR_WHITE);
	cont_debug_clear();

	debug_line_remove_timer = cont_timer_create(DEBUG_LINE_VIEWTIME);
}

void cont_debug(char * text)
{
	cont_timer_reset(debug_line_remove_timer);

	if (debug_cur_line < DEBUG_LINES) {
		strcpy(debug_lines[debug_cur_line], text);
		debug_cur_line++;
	}
	else {
		for (unsigned int i = 0; i < DEBUG_LINES-1; i++) {
			strcpy(debug_lines[i], debug_lines[i+1]);
		}

		strcpy(debug_lines[DEBUG_LINES - 1], text);
	}
}

void cont_debug_draw()
{
	attron(COLOR_PAIR(1));
	
	for (unsigned int i = 0; i < DEBUG_LINES; i++) {
		mvprintw(i, 0, debug_lines[i]);
	}

	attroff(COLOR_PAIR(1));
}

void cont_debug_update()
{
	if (cont_timer_finished_reset(debug_line_remove_timer) && debug_cur_line != 0) {
		// remove line 0, shift all lines down
		for (unsigned int i = 0; i < DEBUG_LINES - 1; i++) {
			strcpy(debug_lines[i], debug_lines[i + 1]);
		}

		strcpy(debug_lines[DEBUG_LINES - 1], "");

		debug_cur_line--;
	}
}

void cont_debug_clear()
{
	debug_cur_line = 0;

	for (unsigned int i = 0; i < DEBUG_LINES; i++) {
		debug_lines[i][0] = '\0';
	}
}