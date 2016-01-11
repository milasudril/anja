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

		PlaybackRange& delayReset()
			{
			m_delay=0;
			return *this;
			}

		void stop()
			{r_current=r_end;}

		uint32_t flagsGet() const
			{return m_flags;}

		PlaybackRange& flagsSet(uint32_t flags)
			{
			m_flags|=flags;
			return *this;
			}

		PlaybackRange& flagsUnset(uint32_t flags)
			{
			m_flags&=~flags;
			return *this;
			}


	private:
		uint32_t m_delay;
		const float* r_begin;
		const float* r_current;
		const float* r_end;
		float m_fs;
		float m_gain;
		uint32_t m_flags;
	};

#endif

