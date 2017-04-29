//@	 {"targets":[{"name":"floatconv.o","type":"object"}]}

#include "floatconv.hpp"
#include "arraydynamicshort.hpp"
#include "localeguard.hpp"

using namespace Anja;

bool Anja::convert(const char* string,double& result)
	{
	auto ptr=string;
	bool got_separator=0;
	String buffer;
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
