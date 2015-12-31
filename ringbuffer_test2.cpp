#ifdef __WAND__
target[name[ringbuffer_test2] type[application] platform[;GNU/Linux]]
#endif

#include "framework/arraydynamicshort.h"
#include <cstdio>

int main()
	{
	int ch_in;
	int x=0;
	int x_prev=-1;
	ArrayDynamicShort<char> buffer;
	while((ch_in=getchar())!=EOF)
		{
		if(ch_in>='\0' && ch_in<=' ')
			{
			if(x==0)
				{
				buffer.append('\0');
				x=atoi(buffer.begin());
				if(x!=x_prev+1)
					{
					printf("Race condition detected\n");
					return 1;
					}
				x_prev=x;
				buffer.clear();
				}
			}
		else
			{
			x=0;
			buffer.append(ch_in);
			}
		}
	return 0;
	}
