#ifdef __WAND__
target[name[minifloat_test] type[application] platform[;GNU/Linux]]
#endif

#include "minifloat.h"
#include <cstdio>

int main()
	{
	for(uint32_t k=0;k<0x72;++k)
		{
		auto x=Minifloat::fromBits(k);
		double y=x;
		printf("%x  %.15g\n",k,y);
		}

	return 0;
	}
