#ifdef __WAND__
target[name[playbackrange.o] type[object]]
#endif

#include "playbackrange.h"
#include "waveform.h"
#include <cstring>

#include <cstdio>

PlaybackRange::PlaybackRange()
	{memset(this,0,sizeof(*this));}

PlaybackRange::PlaybackRange(const Waveform& waveform,uint32_t start_delay):
	 m_delay(start_delay)
	,r_begin(waveform.begin()),r_current(waveform.begin()),r_end(waveform.end())
	,m_fs(waveform.sampleRateGet()),m_gain(waveform.gainGet())
	,m_flags(waveform.flagsGet())
	{
	}

unsigned int PlaybackRange::outputBufferGenerate(float* buffer_out
	,unsigned int n_frames_out)
	{
	buffer_out+=m_delay;
	n_frames_out-=m_delay;
	auto gain=m_gain;
	auto ptr_current=r_current;
	auto ptr_end=r_end;
	auto dir=ptr_current<ptr_end?1:-1;
	while(n_frames_out!=0 && ptr_current!=ptr_end)
		{
		*buffer_out=gain*(*ptr_current);
		ptr_current+=dir;
		++buffer_out;
		--n_frames_out;
		}
	auto N=dir*(ptr_current-r_current);
	r_current=ptr_current;
	delayReset();
	return N;
	}
