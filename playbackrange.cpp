#ifdef __WAND__
target[name[playbackrange.o] type[object]]
#endif

#include "playbackrange.h"
#include "waveform.h"
#include "units.h"
#include "framework/randomgenerator.h"

#include <cstring>
#include <cstdio>

PlaybackRange::PlaybackRange() noexcept
	{memset(this,0,sizeof(*this));}

void PlaybackRange::waveformSet(RandomGenerator& rng,Waveform& waveform
	,uint32_t start_delay,float velocity) noexcept
	{
	m_decay_factor=1.0;
	m_gain_out=1.0;
	r_rng=&rng;
	r_waveform=&waveform;

	m_delay=start_delay;
//	If user tries to load a new waveform in slot while it is playing
//	a SIGSEGV will occur. Therefore, the waveform object needs to be
//	marked as locked before we continue.
	waveform.flagsSet(Waveform::PLAYBACK_RUNNING);

	r_begin=waveform.begin();
	r_current=waveform.begin();
	r_end=waveform.end();
	m_flags=waveform.flagsGet();

	auto gain_random=waveform.gainRandomGet();
	m_gain_random_inst=r_rng->drawUniform(gain_random);
	m_velocity=velocity;
	}

void PlaybackRange::release()
	{
//	Clear lock flag
	r_waveform->flagsUnset(Waveform::PLAYBACK_RUNNING);
	r_waveform=nullptr;
	}

void PlaybackRange::stop(unsigned int delay) noexcept
	{
//	NOTE: If sample rate conversions are introduced, delay has to be converted
//	to other time units, or the pointers refered to by this functions must be
//	the output from the sample rate converter

	auto ptr_current=r_current;
	auto ptr_end=r_end;
	auto dir=ptr_current<ptr_end?1:-1;
	if(dir>0)
		{
		r_end=std::min(ptr_end,ptr_current+delay);
		}
	else
		{
		auto tmp=delay>uintptr_t(ptr_current)? ptr_end : ptr_current-delay;
		r_end=std::max(ptr_end,tmp);
		}
	m_flags&=~Waveform::LOOP;
	}


unsigned int PlaybackRange::outputBufferGenerate(float* buffer_out
	,unsigned int n_frames_out) noexcept
	{
	auto ptr_begin=r_begin;
	auto ptr_end=r_end;
	if(ptr_begin==ptr_end)
		{return 0;}

	auto buffer_out_in=buffer_out;
	buffer_out+=m_delay;
	n_frames_out-=m_delay;

	auto flags=m_flags;
	auto ptr_current=r_current;

	auto gain_random_inst=m_gain_random_inst;
	auto gain_init=r_waveform->gainGet();
	auto gain_random=r_waveform->gainRandomGet();

	auto gain=dBToAmplitude(gain_random_inst + gain_init)*m_velocity;
	auto decay_factor=m_decay_factor;
	auto gain_out=m_gain_out;
	auto dir=ptr_current<ptr_end?1:-1;
	while(n_frames_out!=0 && ptr_current!=ptr_end)
		{
		*buffer_out=gain_out*gain*(*ptr_current);
		++buffer_out;
		gain_out*=decay_factor;
		--n_frames_out;
		ptr_current+=dir;

	//	TODO Use a third cursor for loops, so there can be a separate decay
	//	phase. The tricky part is to find space in the UI (WaveformRangeView)
		if(ptr_current==ptr_end)
			{
			ptr_current=(flags&Waveform::LOOP)? ptr_begin : ptr_current;
			gain_random_inst=(flags&Waveform::GAIN_ONLOOP_SET)?
				r_rng->drawUniform(gain_random) : gain_random_inst;
			}

		if(gain_out<1e-4)
			{stop();}
		}
	m_gain_out=gain_out;
	r_current=ptr_current;
	m_gain_random_inst=gain_random_inst;
	delayReset();
	return buffer_out-buffer_out_in;
	}
