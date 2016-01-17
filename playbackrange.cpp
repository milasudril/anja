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
	,uint32_t start_delay) noexcept
	{
	r_rng=&rng;
	r_waveform=&waveform;

	m_delay=start_delay;
//	If user tries to load a new waveform in slot while it is playing
//	a SIGSEGV will occur. Therefore, the waveform object needs to be
//	marked as locked before we continue.
	waveform.flagsSet(Waveform::LOCKED);

	r_begin=waveform.begin();
	r_current=waveform.begin();
	r_end=waveform.end();
	m_flags=waveform.flagsGet();

	auto gain_random=waveform.gainRandomGet();
	m_gain_random_inst=r_rng->drawUniform(gain_random);
	}

void PlaybackRange::release()
	{
//	Clear lock flag
	r_waveform->flagsUnset(Waveform::LOCKED);
	r_waveform=nullptr;
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

	auto gain=dBToAmplitude(gain_random_inst + gain_init);

	auto dir=ptr_current<ptr_end?1:-1;
	while(n_frames_out!=0 && ptr_current!=ptr_end)
		{
		*buffer_out=gain*(*ptr_current);
		++buffer_out;
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
		}
	r_current=ptr_current;
	m_gain_random_inst=gain_random_inst;
	delayReset();
	return buffer_out-buffer_out_in;
	}
