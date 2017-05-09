// INCLUDES
#include "curses.h"

#include "contipede_debris.h"

// DEFINITIONS
#define DEBRIS_LIMIT 100
#define DEBRIS_DMGCOLOR0 COLOR_WHITE
#define DEBRIS_DMGCOLOR1 COLOR_BLUE
#define DEBRIS_DMGCOLOR2 COLOR_MAGENTA
#define DEBRIS_DMGCOLOR3 COLOR_RED

typedef struct
{
	int x, y;
	int health, maxhealth;
	char icon;
	int used;
} debris_data;

debris_data debris_data_array[DEBRIS_LIMIT];