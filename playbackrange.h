#ifdef __WAND__
target[name[playbackrange.h] type[include]]
dependency[playbackrange.o]
#endif

#ifndef PLAYBACKRANGE_H
#define PLAYBACKRANGE_H

#include <cstdint>

class Waveform;

class alignas(32) PlaybackRange
	{
	public:
		PlaybackRange();

		PlaybackRange(const Waveform& waveform,uint32_t start_delay);

		unsigned int outputBufferGenerate(float* buffer_out
			,unsigned int n_frames_out);

		bool valid() const
			{return r_begin!=r_end;}

		void delayReset()
			{m_delay=0;}

		void reset()
			{r_end=r_begin;}

	private:
		uint32_t m_delay;
		const float* r_begin;
		const float* r_current;
		const float* r_end;
		float m_fs;
		float m_gain;
	};

#endif

