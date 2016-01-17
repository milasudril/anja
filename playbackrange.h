#ifdef __WAND__
target[name[playbackrange.h] type[include]]
dependency[playbackrange.o]
#endif

#ifndef PLAYBACKRANGE_H
#define PLAYBACKRANGE_H

#include "waveform.h"

class RandomGenerator;

class alignas(32) PlaybackRange
	{
	public:
		PlaybackRange() noexcept;

		void waveformSet(RandomGenerator& rng,Waveform& waveform
			,uint32_t start_delay) noexcept;

		bool valid() const noexcept
			{return r_waveform!=nullptr;}

		void release();

		unsigned int outputBufferGenerate(float* buffer_out
			,unsigned int n_frames_out) noexcept;

		bool done() const noexcept
			{return r_current==r_end;}

		void stop() noexcept
			{r_begin=r_end;}

		PlaybackRange& delayReset() noexcept
			{
			m_delay=0;
			return *this;
			}

		uint32_t flagsGet() const noexcept
			{return m_flags;}

		PlaybackRange& flagsSet(uint32_t flags) noexcept
			{
			m_flags|=flags;
			return *this;
			}

		PlaybackRange& flagsUnset(uint32_t flags) noexcept
			{
			m_flags&=~flags;
			return *this;
			}

		uint32_t channelGet() const noexcept
			{return r_waveform->channelGet();}


	private:
		RandomGenerator* r_rng;
		Waveform* r_waveform;
		uint32_t m_delay;
		const float* r_begin;
		const float* r_current;
		const float* r_end;
		uint32_t m_flags;
		float m_gain_random_inst;
	};

#endif

