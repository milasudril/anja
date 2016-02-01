#ifdef __WAND__
target[name[optionstring.o] type[object]]
#endif

#include "optionstring.h"

ArrayDynamicShort<char> stringFromOptions(unsigned int option_bits
	,const char* const* option_names)
	{
	ArrayDynamicShort<char> ret;

	if(*option_names==nullptr)
		{
		ret.append('\0');
		return std::move(ret);
		}

	auto mask=0x1u;

	if(option_bits&mask)
		{ret.append(*option_names);}

	++option_names;
	mask<<=1;

	while(*option_names!=nullptr)
		{
		if(option_bits&mask)
			{ret.append('|').append(*option_names);}

		++option_names;
		mask<<=1;
		}

	ret.append('\0');
	return std::move(ret);
	}

unsigned int optionsFromString(const char* string
	,const char* const* option_names)
	{
	return 0;
	}
