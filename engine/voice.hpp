//@	{
//@	 "targets":[{"name":"voice.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"voice.o","rel":"implementation"}]
//@	}

#ifndef ANJA_VOICE_HPP
#define ANJA_VOICE_HPP

namespace Anja
	{
	class Voice
		{
		public:
			Voice()=default;

			void generate(float* buffer_out,int n_frames);

		private:
			float velocity;
			float* r_gain;
			float* r_gain_random;
			float* r_pos_current;

			float* r_loop_begin;
			float* r_loop_end;
			float* r_end;

			static constexpr int LOOP=1;
			static constexpr int SET_GAIN_ON_LOOP=2;
			int m_flags;
		};
	}

#endif // ANJA_VOICE_HPP
