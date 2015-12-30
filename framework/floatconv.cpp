#ifdef __WAND__
target[name[floatconv.o] type[object]]
#endif

#include "floatconv.h"
#include "arraydynamicshort.h"
#include "localeguard.h"

bool convert(const char* string,double& result)
	{
	auto ptr=string;
	bool got_separator=0;
	ArrayDynamicShort<char> buffer;
	while(*ptr!='\0')
		{
		if(*ptr>=1 && *ptr<=' ')
			{++ptr;}
		else
			{
			switch(*ptr)
				{
				case '.':
				case ',':
					if(got_separator)
						{return 0;}
					buffer.append('.');
					got_separator=1;
					++ptr;
					break;
				default:
					buffer.append(*ptr);
					++ptr;
				}
			}
		}
	buffer.append('\0');

	char* ptr_end;
	auto text=buffer.begin();
	LocaleGuard locale("C");
	result=strtod(text,&ptr_end);
	if(ptr_end!=text)
		{return 1;}
	return 0;
	}
