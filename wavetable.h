#ifdef __WAND__
target[name[wavetable.h] type[include]]
#endif

#ifndef WAVETABLE_H
#define WAVETABLE_H

#include "waveform.h"
#include "framework/array_fixed.h"

class Wavetable:public ArrayFixed<Waveform,128>
	{};

#endif
