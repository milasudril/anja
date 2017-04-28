//@	{
//@	 "targets":[{"name":"colorstring.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"colorstring.o","rel":"implementation"}]
//@	}

#ifndef ANJA_COLORSTRING_HPP
#define ANJA_COLORSTRING_HPP

#include "arrayfixed.hpp"
#include "color.hpp"

namespace Anja
	{
	class ColorString:public ArrayFixed<char,64>
		{
		public:
			ColorString(const ColorRGBA& color);
		};

	ColorRGBA colorFromString(const char* colorstring);
	}

#endif
