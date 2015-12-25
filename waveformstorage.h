#ifdef __WAND__
target[name[waveform_storage.h] type[include]]
#endif

#ifndef WAVEFORMSTORAGE_H
#define WAVEFORMSTORAGE_H

#include "waveform.h"
#include "framework/array_dynamic_short.h"

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

		void clear()
			{m_data.clear();}
	};

#endif
