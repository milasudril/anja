//@	{"targets":[{"name":"voice.o","type":"object"}]}

#include "voice.hpp"
#include "../common/units.hpp"
#include "../sessiondata/waveform.hpp"

using namespace Anja;

Voice::Voice(const Waveform& waveform,float velocity) noexcept
	{
	m_velocity=velocity;
	m_gain=dBToAmplitude( waveform.gainGet() );
	m_gain_random=dBToAmplitude(waveform.gainRandomGet());

	r_pos_current=waveform.begin();
	r_loop_begin=r_pos_current; //For now;
	r_loop_end=waveform.end(); //For now;
	r_end=waveform.end();
	m_flags=0;
	}

void Voice::generate(float* buffer_out,int n_frames) noexcept
	{
	auto g=m_gain*m_velocity;
	while(n_frames!=0 && r_pos_current!=r_end)
		{
		*buffer_out+=g*(*r_pos_current);
		++r_pos_current;
		if(r_pos_current==r_loop_end && (m_flags&LOOP))
			{
			r_pos_current=r_loop_begin;
			if(m_flags&SET_GAIN_ON_LOOP)
				{
				//TODO set gain from random generator...
				}
			}

		++buffer_out;
		--n_frames;
		}
	}
