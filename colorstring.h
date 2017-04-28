#ifdef __WAND__
target[name[colorstring.h] type[include]]
dependency[colorstring.o]
#endif

#ifndef COLORSTRING_H
#define COLORSTRING_H

#include "framework/array_fixed.h"
#include "framework/color.h"

class ColorString:public ArrayFixed<char,64>
	{
	public:
		ColorString(const ColorRGBA& color);
	};

ColorRGBA colorFromString(const char* colorstring);

#endif
