//@	{
//@	 "targets":[{"name":"meansquare.hpp","type":"include"}]
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

			template<class ProgressCallback>
			void compute(const float* buffer_in,float* buffer_out,size_t N_samples
				,ProgressCallback& cb);

		private:
			DelayLine<double> m_values;
			double m_sum;
		};

	MeanSquare::MeanSquare(int32_t N_samples):m_values(N_samples),m_sum(0.0)
		{
		m_values.fill(0);
		}

	template<class ProgressCallback>
	void MeanSquare::compute(const float* buffer_in,float* buffer_out,size_t N_samples
		,ProgressCallback& cb)
		{
		auto sum=m_sum;
		auto delay=m_values.length();
		auto start=buffer_in;
		auto N_tot=N_samples;
		while(N_samples!=0)
			{
			auto value_in=*buffer_in;
			value_in*=value_in;
			m_values.push_back(value_in);
			sum+=value_in - m_values.front();
			*buffer_out=float(sum/delay);
			++buffer_out;
			++buffer_in;
			cb.progressCompute(*this,static_cast<float>(buffer_in - start)
				/static_cast<float>(N_tot));
			--N_samples;
			}
		sum=m_sum;
		}
	}

#endif
