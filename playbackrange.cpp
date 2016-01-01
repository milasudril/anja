#ifdef __WAND__
target[name[playbackrange.o] type[object]]
#endif

#include "playbackrange.h"
#include "waveform.h"

PlaybackRange::PlaybackRange(const Waveform& waveform):
	 r_begin(waveform.begin()),r_current(nullptr),r_end(waveform.end())
	,m_fs(waveform.sampleRateGet()),m_gain(waveform.gainGet())
	{}

unsigned int PlaybackRange::outputBufferGenerate(float* buffer_out
	,unsigned int n_frames_out,double fs_out)
	{
	auto gain=m_gain;
	auto ptr_current=r_current;
	auto ptr_end=r_end;
	while(n_frames_out!=0 && ptr_current!=ptr_end)
		{
		*buffer_out=gain*(*ptr_current);
		++ptr_current;
		++buffer_out;
		--n_frames_out;
		}
	auto N=ptr_current-r_current;
	r_current=ptr_current;
	return N;
	}
