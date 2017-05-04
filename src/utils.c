#include "utils.h"

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

void u_waitms(int numMS) {
	PSLEEP(numMS * MSTIME);
}