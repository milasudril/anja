//	{"targets":[{"name":"wavetable.hpp","type":"include"}]}

#ifndef ANJA_WAVETABLE_H
#define ANJA_WAVETABLE_H

#include "waveform.hpp"
#include "arrayfixed.hpp"

namespace Anja
	{
	class Wavetable final:public ArrayFixed<Waveform,128>
		{};
	}

#endif
