#ifdef __WAND__
target[name[clock.o] type[object] platform[;GNU/Linux]]
#endif

#include "clock.h"
#include <ctime>

double clockGet()
	{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW,&now);
	return 1e-9*now.tv_nsec+now.tv_sec;
	}
