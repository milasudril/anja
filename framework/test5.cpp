#ifdef __WAND__
target[type[application] name[test5]]
#endif

#include "color.h"
#include <cstdio>


void print(const ColorRGBA& c)
	{
	printf("red: %.7g, green: %.7g, blue: %.7g, luma: %.7g\n"
		,c.red,c.green,c.blue,luma709(c));
	}

int main()
	{
	auto luma_green=luma709({0.0f,1.0f,0.0f,1.0f});

	size_t N=12;
	for(size_t k=0;k<N;++k)
		{print(ColorHSLA::fromHueAndLuma(float(k)/N,luma_green));}

	return 0;
	}
