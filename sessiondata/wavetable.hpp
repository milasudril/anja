//	{"targets":[{"name":"wavetable.hpp","type":"include"}]}

#ifndef ANJA_WAVETABLE_HPP
#define ANJA_WAVETABLE_HPP

#include "waveform.hpp"
#include "arrayfixed.hpp"

namespace Anja
	{
	class Wavetable final:public ArrayFixed<Waveform,128>
		{};
	}

#endif
