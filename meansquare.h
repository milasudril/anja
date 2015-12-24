#ifdef __WAND__
target[name[meansquare.h] type[include]]
dependency[meansquare.o]
#endif

#ifndef MEANSQUARE_H
#define MEANSQUARE_H

#include "delayline.h"

class MeanSquare
	{
	public:
		MeanSquare(uint32_t N_samples);
		void compute(const float* buffer_in,float* buffer_out,size_t N_samples);

	private:
		DelayLine<double> m_values;
		double m_sum;
	};

#endif
