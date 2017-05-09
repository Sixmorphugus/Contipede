#include "contipede_platform.h"

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

int cont_plat_timeout_from_speed(int speed)
{
	return 1000 / speed;
}
