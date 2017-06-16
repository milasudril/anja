//@	{"targets":[{"name":"wavetable.hpp","type":"include"}]}

#ifndef ANJA_WAVETABLE_HPP
#define ANJA_WAVETABLE_HPP

#include "waveform.hpp"
#include "../common/arrayfixed.hpp"

namespace Anja
	{
	class Wavetable final:public ArrayFixed<Waveform,128>
		{};
	}

#endif
