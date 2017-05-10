#include "contipede_platform.h"

#include <math.h>

#define FRAMETIME 10

// are we using the windows sleep function or the unix one?
// unix uses microseconds, windows uses milliseconds
#ifdef WIN32
#include "windows.h"
#define PSLEEP Sleep
#define MSTIME 1
#else
#include "unistd.h"
#define PSLEEP usleep;
#define MSTIME 1000
#endif

void cont_plat_waitms(int numMS) {
	PSLEEP(numMS * MSTIME);
}

int cont_plat_get_frametime() {
	return FRAMETIME;
}

int cont_plat_speed_increment(int input)
{
	if (input > 0) return 1;
	if (input < 0) return -1;

	return 0;
}

int cont_plat_timeout_from_speed(int speed)
{
	if (speed == 0)
		return 0;

	return 1000 / abs(speed);
}

void cont_plat_beep()
{
	//mvprintw(0, getmaxx()-1, "\a"); removed in windows 10
}
