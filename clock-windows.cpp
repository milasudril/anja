#ifdef __WAND__
target[name[clock.o] type[object] platform[;Windows] dependency[winmm;external]]
#endif

#include "clock.h"
#include <windows.h>

double clockGet()
	{
	return 1e-3*timeGetTime();
	}
