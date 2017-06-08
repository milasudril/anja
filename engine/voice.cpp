//@	{"targets":[{"name":"voice.o","type":"object"}]}

#include "voice.hpp"
#include "../common/units.hpp"
#include "../sessiondata/waveform.hpp"

using namespace Anja;

Voice::Voice(const Waveform& waveform,int channel,float velocity,int start_offset) noexcept
	{
	m_velocity=velocity;
	m_gain=dBToAmplitude( waveform.gainGet() );
	m_gain_random=dBToAmplitude(waveform.gainRandomGet());

	r_pos_current=waveform.pointer<Waveform::Cursor::BEGIN>();
	r_loop_begin=waveform.pointer<Waveform::Cursor::BEGIN_LOOP>();
	r_loop_end=waveform.pointer<Waveform::Cursor::END_LOOP>();
	r_end=waveform.pointer<Waveform::Cursor::END>();
	m_pos_offset=start_offset;
	m_state=State::BEGIN;
	m_flags=waveform.flagsGet();
	m_channel=channel;
	}

Voice& Voice::stop(int offset) noexcept
	{
	if(m_flags&Waveform::SUSTAIN)
		{m_flags&=~Waveform::LOOP;}
	else
		{
		m_pos_offset=offset;
		m_state=State::END;
		}
	return *this;
	}

void Voice::generate(float* buffer_out,int n_frames) noexcept
	{
	switch(m_state)
		{
		case State::BEGIN:
			buffer_out+=m_pos_offset;
			n_frames-=m_pos_offset;
			m_state=State::RUNNING;
			break;
		case State::END:
			n_frames=std::min(m_pos_offset,n_frames);
			m_state=State::DONE;
			break;
		case State::DONE:
			return;
		default:
			break;
		}

	auto g=m_gain*m_velocity;
	while(n_frames!=0 && r_pos_current!=r_end)
		{
		*buffer_out+=g*(*r_pos_current);
		++r_pos_current;
		if(r_pos_current==r_loop_end && (m_flags&Waveform::LOOP))
			{
			r_pos_current=r_loop_begin;
			if(m_flags&Waveform::GAIN_ONLOOP_SET)
				{
				//TODO set gain from random generator...
				}
			}

		++buffer_out;
		--n_frames;
		}
	}
