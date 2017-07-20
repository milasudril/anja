//@	{"targets":[{"name":"wavefileinfo.hpp","type":"include"}]}

#ifndef ANJA_WAVEFILEINFO_H
#define ANJA_WAVEFILEINFO_H

#include <cstdint>

namespace Anja
	{
	struct WavefileInfo
		{
		uint32_t n_frames;
		uint32_t n_channels;
		double fs;
		};
	}

#endif
