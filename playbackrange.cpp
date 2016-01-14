#ifdef __WAND__
target[name[playbackrange.o] type[object]]
#endif

#define _GLIBCXX_DEBUG

#include "playbackrange.h"
#include "waveform.h"
#include "units.h"
#include "framework/randomgenerator.h"

#include <cstring>
#include <cstdio>

PlaybackRange::PlaybackRange()
	{memset(this,0,sizeof(*this));}

void PlaybackRange::waveformSet(RandomGenerator& rng,const Waveform& waveform
	,uint32_t start_delay)
	{
	r_rng=&rng;
	m_delay=start_delay;
	r_begin=waveform.begin();
	r_current=waveform.begin();
	r_end=waveform.end();
	m_fs=waveform.sampleRateGet();
	m_gain_init=waveform.gainGet();
	m_gain_random=waveform.gainRandomGet();
	m_gain=dBToAmplitude( m_gain_init + rng.drawUniform( m_gain_random ) );
	m_flags=waveform.flagsGet();
	}

unsigned int PlaybackRange::outputBufferGenerate(float* buffer_out
	,unsigned int n_frames_out)
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
	auto gain=m_gain;
	auto gain_random=m_gain_random;

	auto dir=ptr_current<ptr_end?1:-1;
	while(n_frames_out!=0 && ptr_current!=ptr_end)
		{
		*buffer_out=gain*(*ptr_current);
		++buffer_out;
		--n_frames_out;
		ptr_current+=dir;
		if(ptr_current==ptr_end)
			{
			ptr_current=(flags&Waveform::LOOP)? ptr_begin : ptr_current;
			gain=(flags&Waveform::SET_GAIN_ON_LOOP)?
				dBToAmplitude(m_gain_init + r_rng->drawUniform(gain_random)):gain;
			}
		}
	r_current=ptr_current;
	m_gain=gain;
	delayReset();
	return buffer_out-buffer_out_in;
	}
