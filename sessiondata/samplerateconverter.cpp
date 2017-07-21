//@	 {
//@	"targets":
//@		[{
//@		"name":"samplerateconverter.o","type":"object","pkgconfig_libs":["samplerate"]
//@		}]
//@	}

#include "samplerateconverter.hpp"
#include "../common/error.hpp"
#include <samplerate.h>

using namespace Anja;

class SampleRateConverter::Impl
	{
	public:
		Impl(Converter conv,double ratio);

		void compute(const float* buffer_in,size_t& size_in,bool last
			,float* buffer_out,size_t& size_out) noexcept;

		void reset() noexcept
			{src_reset(m_state);}

		void ratio(double r) noexcept
			{m_ratio=r;}

	private:
		SRC_STATE* m_state;
		double m_ratio;
	};

SampleRateConverter::SampleRateConverter(Converter conv,double ratio)
	{m_impl.reset( new Impl(conv,ratio) );}

SampleRateConverter::~SampleRateConverter()
	{}

SampleRateConverter& SampleRateConverter::ratio(double r) noexcept
	{
	m_impl->ratio(r);
	return *this;
	}

SampleRateConverter& SampleRateConverter::reset() noexcept
	{
	m_impl->reset();
	return *this;
	}

void SampleRateConverter::compute(const float* buffer_in,size_t& size_in
	,bool last,float* buffer_out,size_t& size_out) noexcept
	{
	m_impl->compute(buffer_in,size_in,last,buffer_out,size_out);
	}

static int converter(SampleRateConverter::Converter conv)
	{
	switch(conv)
		{
		case SampleRateConverter::Converter::SINC_BEST:
			return SRC_SINC_BEST_QUALITY;
		case SampleRateConverter::Converter::SINC_MEDIUM:
			return SRC_SINC_MEDIUM_QUALITY;
		case SampleRateConverter::Converter::SINC_FASTEST:
			return SRC_SINC_FASTEST;
		case SampleRateConverter::Converter::LINEAR:
			return SRC_LINEAR;
		case SampleRateConverter::Converter::ZERO_ORDER_HOLD:
			return SRC_ZERO_ORDER_HOLD;
		}
	return 0;
	}

SampleRateConverter::Impl::Impl(Converter conv,double ratio)
	{
	int err;
	m_state=src_new(converter(conv),1,&err);
	if(m_state==NULL)
		{throw Error("Failed to create a new sample rate converter.");}
	m_ratio=ratio;
	}

void SampleRateConverter::Impl::compute(const float* buffer_in,size_t& size_in
	,bool last,float* buffer_out,size_t& size_out) noexcept
	{
	SRC_DATA data{};
	data.data_in=const_cast<float*>( buffer_in ); //OK?
	data.data_out=buffer_out;
	data.input_frames=size_in;
	data.output_frames=size_out;
	data.input_frames_used=0;
	data.output_frames_gen=0;
	data.end_of_input=last;
	data.src_ratio=m_ratio;
	src_process(m_state,&data);
	size_in=data.input_frames_used;
	size_out=data.output_frames_gen;
	}
