#ifdef __WAND__
target[name[waveform.o] type[object]]
#endif

#include "waveform.h"
#include "optionstring.h"

const char* Waveform::FLAG_NAMES[]=
	{"Loop","Sustain","Readonly","Set gain on loop",nullptr};
