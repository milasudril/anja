#ifdef __WAND__
target[name[waveformrange.o] type[object]]
#endif

#include "waveformrange.h"
#include "framework/array_fixed.h"

constexpr ArrayFixed<float,2> data{1.0e-7f,1.0e-7f};

WaveformRange WaveformRange::s_null{data.begin(),data.length(),1000.0f};
