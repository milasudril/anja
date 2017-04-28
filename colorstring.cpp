#ifdef __WAND__
target[name[colorstring.o] type[object]]
#endif

#include "colorstring.h"
#include "framework/arraydynamicshort.h"
#include "framework/floatconv.h"
#include <cstdio>


ColorString::ColorString(const ColorRGBA& color)
	{
	sprintf(begin(),"%.7g; %.7g; %.7g; %.7g"
		,color.red,color.green,color.blue,color.alpha);
	}


ColorRGBA colorFromString(const char* colorstring)
	{
	ArrayDynamicShort<char> buffer;
	float values[4]={0.0f,0.0f,0.0f,1.0f};
	uint8_t count=0;
	while(*colorstring!='\0' && count!=4)
		{
		switch(*colorstring)
			{
			case ';':
				buffer.append('\0');
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
		{
		buffer.append('\0');
		values[count]=convert(buffer.begin());
		}

	return {values[0],values[1],values[2],values[3]};
	}