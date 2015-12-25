#ifdef __WAND__
target[name[waveformrange.h] type[include]]
dependency[waveformrange.o]
#endif

#ifndef WAVEFORMRANGE_H
#define WAVEFORMRANGE_H

#include "waveform.h"
#include "waveformstorage.h"
#include "framework/array_non_owning.h"

class WaveformRange:public Waveform< ArrayNonOwning<float> >
	{
	public:
		constexpr WaveformRange(const float* buffer,uint32_t length,float fs):
			Waveform<ArrayNonOwning<float> >(buffer,length,fs)
			{}

		WaveformRange(WaveformStorage& storage):
			Waveform<ArrayNonOwning<float> >(storage.beginFull()
				,storage.lengthFull(),storage.sampleRateGet())
			{}

		static WaveformRange s_null;
	};

#endif
