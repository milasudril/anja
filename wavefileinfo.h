#ifdef __WAND__
target[name[wavefileinfo.h] type[include]]
#endif

#ifndef WAVEFILEINFO_H
#define WAVEFILEINFO_H

#include <cstdint>

struct WavefileInfo
	{
	uint32_t n_frames;
	float fs;
	uint32_t n_channels;
	};

#endif
