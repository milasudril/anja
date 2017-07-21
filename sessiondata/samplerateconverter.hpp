//@	{
//@	 "targets":[{"name":"samplerateconverter.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"samplerateconverter.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SAMPLERATECONVERTER_HPP
#define ANJA_SAMPLERATECONVERTER_HPP

#include <memory>

namespace Anja
	{
	class SampleRateConverter
		{
		public:
			enum class Converter:int{SINC_BEST,SINC_MEDIUM,SINC_FASTEST,LINEAR,ZERO_ORDER_HOLD};

			explicit SampleRateConverter(Converter conv,double ratio);
			~SampleRateConverter();

			SampleRateConverter& ratio(double r) noexcept;
			SampleRateConverter& reset() noexcept;

			void compute(const float* buffer_in,size_t& size_in
				,bool last,float* buffer_out,size_t& size_out) noexcept;

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif // ANJA_SAMPLERATECONVERTER_HPP
