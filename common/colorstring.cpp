//@	 {"targets":[{"name":"colorstring.o","type":"object"}]}

#include "colorstring.hpp"
#include "string.hpp"
#include "floatconv.hpp"
#include <cstdio>

using namespace Anja;

ColorString::ColorString(const ColorRGBA& color)
	{
	sprintf(begin(),"%.7g; %.7g; %.7g; %.7g"
		,color.red,color.green,color.blue,color.alpha);
	}

ColorRGBA Anja::colorFromString(const char* colorstring)
	{
	String buffer;
	float values[4]={0.0f,0.0f,0.0f,1.0f};
	uint8_t count=0;
	while(*colorstring!='\0' && count!=4)
		{
		switch(*colorstring)
			{
			case ';':
				values[count]=convert(buffer.begin());
				++count;
				buffer.clear();
				break;
			default:
				buffer.append(*colorstring);
				break;
			}
		++colorstring;
		}
	if(count!=4)
		{values[count]=convert(buffer.begin());}

	return ColorRGBA{values[0],values[1],values[2],values[3]};
	}
