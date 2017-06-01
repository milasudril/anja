//@	{"targets":[{"name":"voice.o","type":"object"}]}

#include "voice.hpp"
#include "../common/units.hpp"

using namespace Anja;

void Voice::generate(float* buffer_out,int n_frames)
	{
	auto g=dBToAmplitude(*r_gain);

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
