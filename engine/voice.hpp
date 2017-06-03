//@	{
//@	 "targets":[{"name":"voice.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"voice.o","rel":"implementation"}]
//@	}

#ifndef ANJA_VOICE_HPP
#define ANJA_VOICE_HPP

#include <cstdint>

namespace Anja
	{
	class Waveform;

	class Voice
		{
		public:
			Voice() noexcept:r_pos_current(nullptr),r_end(nullptr){}

			explicit Voice(Waveform&& waveform)=delete;
			explicit Voice(const Waveform& waveform,float velocity,int start_offset) noexcept;

			void generate(float* buffer_out,int n_frames) noexcept;

			bool done() const noexcept
				{return r_pos_current==r_end;}

			Voice& stop(int offset) noexcept
				{
				if(m_flags&SUSTAIN)
					{m_flags&=~LOOP;}
				else
					{
					m_pos_offset=offset;
					m_state=State::END;
					}
				return *this;
				}

		private:
			float m_velocity;
			float m_gain;
			float m_gain_random;
			const float* r_pos_current;

			const float* r_loop_begin;
			const float* r_loop_end;
			const float* r_end;
			int m_pos_offset;

			static constexpr uint16_t LOOP=1;
			static constexpr uint16_t SET_GAIN_ON_LOOP=2;
			static constexpr uint16_t SUSTAIN=4;
			uint16_t m_flags;
			enum class State:uint16_t{BEGIN,RUNNING,END};
			State m_state;
		};
	}

#endif // ANJA_VOICE_HPP
