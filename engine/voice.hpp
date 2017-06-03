//@	{
//@	 "targets":[{"name":"voice.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"voice.o","rel":"implementation"}]
//@	}

#ifndef ANJA_VOICE_HPP
#define ANJA_VOICE_HPP

namespace Anja
	{
	class Waveform;

	class Voice
		{
		public:
			Voice() noexcept:r_pos_current(nullptr),r_end(nullptr){}

			explicit Voice(Waveform&& waveform)=delete;
			explicit Voice(const Waveform& waveform,float velocity) noexcept;

			void generate(float* buffer_out,int n_frames) noexcept;

			bool done() const noexcept
				{return r_pos_current==r_end;}

			Voice& stop() noexcept
				{
				if(m_flags&SUSTAIN)
					{m_flags&=~LOOP;}
				else
					{r_pos_current=r_end;}
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

			static constexpr int LOOP=1;
			static constexpr int SET_GAIN_ON_LOOP=2;
			static constexpr int SUSTAIN=4;
			int m_flags;
		};
	}

#endif // ANJA_VOICE_HPP
