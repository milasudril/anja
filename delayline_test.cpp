#ifdef __WAND__
target[name[delayline_test] type[application]]
#endif

#include "delayline.h"
#include <cstdint>
#include <cstdlib>
#include <cstdio>

int main()
	{
	DelayLine<float> buffer(4);
	buffer.fill(0);
	float sum=0.0f;
	float value_in=1.0f;
	for(size_t k=0;k<3*buffer.length();++k)
		{
		sum+=value_in - buffer.front();
		printf("%.7g\n",sum/4);
		buffer.push_back(value_in);
		}

	return 0;
	}
