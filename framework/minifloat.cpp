#ifdef __WAND__
target[name[minifloat.o] type[object]]
#endif

#include "minifloat.h"

Minifloat::operator double() const
	{
	union
		{
		double x;
		uint64_t value;
		} ret;
	ret.value=uint64_t(m_bits&0x80)<<56;

	auto exp_biased=(m_bits&0x70)>>4;
	auto mantissa=m_bits&0xf;
	switch(exp_biased)
		{
		case 0x7: //inf or nan
			ret.value|=0x7ff0000000000000|mantissa;
			break;

		default:
			{
			auto exponent=uint64_t( int( exp_biased )-3 + 1023);
			auto mantissa_out=(exp_biased==0 && mantissa==0)?
				0:uint64_t(mantissa)<<(64 - (12 + 4));
			exponent<<=(64 - 12);

			ret.value|=mantissa_out|exponent;
			}
		}
	return ret.x;
	}
