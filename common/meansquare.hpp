//@	{
//@	 "targets":[{"name":"meansquare.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"meansquare.o","rel":"implementation"}]
//@	}

#ifndef ANJA_MEANSQUARE_HPP
#define ANJA_MEANSQUARE_HPP

#include "delayline.hpp"

namespace Anja
	{
	class MeanSquare
		{
		public:
			MeanSquare(int32_t N_samples);
			void compute(const float* buffer_in,float* buffer_out,size_t N_samples);

		private:
			DelayLine<double> m_values;
			double m_sum;
		};
	}

#endif
