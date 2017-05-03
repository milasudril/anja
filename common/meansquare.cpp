//@	{"targets":[{"name":"meansquare.o","type":"object"}]}

#include "meansquare.hpp"

using namespace Anja;

MeanSquare::MeanSquare(int32_t N_samples):m_values(N_samples),m_sum(0.0)
	{
	m_values.fill(0);
	}

void MeanSquare::compute(const float* buffer_in,float* buffer_out,size_t N_samples)
	{
	auto sum=m_sum;
	auto delay=m_values.length();
	while(N_samples!=0)
		{
		auto value_in=*buffer_in;
		value_in*=value_in;
		m_values.push_back(value_in);
		sum+=value_in - m_values.front();
		*buffer_out=float(sum/delay);
		++buffer_out;
		++buffer_in;
		--N_samples;
		}
	sum=m_sum;
	}
