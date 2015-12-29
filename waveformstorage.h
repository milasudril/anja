#ifdef __WAND__
target[name[waveform_storage.h] type[include]]
#endif

#ifndef WAVEFORMSTORAGE_H
#define WAVEFORMSTORAGE_H

#include "waveform.h"
#include "framework/arraydynamicshort.h"

class WaveformStorage:public Waveform< ArrayDynamicShort<float> >
	{
	public:
		WaveformStorage():Waveform< ArrayDynamicShort<float> >(nullptr,0,48000)
			{}

		WaveformStorage& append(float x)
			{
			m_data.append(x);
			return *this;
			}

		WaveformStorage& append(const float* x,uint32_t n)
			{
			m_data.append(x,n);
			return *this;
			}

		WaveformStorage& capacitySet(uint32_t capacity_new)
			{
			m_data.capacitySet(capacity_new);
			return *this;
			}

		void clear()
			{m_data.clear();}
	};

#endif
