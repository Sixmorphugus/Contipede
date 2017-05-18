#pragma once

// This file is here so that all the IDs for colors we are using are all a) allocated by the compiler and b) all in the same place
enum {
	cont_colorpair_unused = 0, // you CANNOT use pair 0 in curses
	cont_colorpair_debug,
	cont_colorpair_bullet,
	cont_colorpair_debris_dmgnone,
	cont_colorpair_debris_dmg1,
	cont_colorpair_debris_dmg2,
	cont_colorpair_debris_dmg3,
	cont_colorpair_centipede,
	cont_colorpair_menuback,
	cont_colorpair_score
};