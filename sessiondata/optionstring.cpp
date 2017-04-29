//@	 {"targets":[{"name":"optionstring.o","type":"object"}]}

#include "optionstring.hpp"

using namespace Anja;

#include <cstring>

String Anja::stringFromOptions(unsigned int option_bits
	,const char* const* option_names)
	{
	String ret;

	if(*option_names==nullptr)
		{
		ret.append('\0');
		return std::move(ret);
		}

	auto mask=0x1u;

	while(*option_names!=nullptr)
		{
		if(option_bits&mask)
			{
			if(ret.length() > 0)
				{ret.append('|');}
			ret.append(String(*option_names)).truncate();
			}

		++option_names;
		mask<<=1;
		}
	ret.append('\0');
	return std::move(ret);
	}

unsigned int Anja::optionsFromString(const char* string
	,const char* const* option_names)
	{
	unsigned int ret=0;
	auto ptr=string;
//	TODO: This function can be implemented without a temporary buffer, by
//	comparing ranges instead of full strings.
	String buffer_temp;
	int state=0;
	while(*ptr!='\0')
		{
		auto ch_in=*ptr;
		switch(state)
			{
			case 0:
				if(ch_in>=0 && ch_in<=' ')
					{state=0;}
				else
					{
					buffer_temp.append(ch_in);
					state=1;
					}
				break;

			case 1:
				if(ch_in>=0 && ch_in<=' ')
					{
					buffer_temp.append(ch_in);
					state=2;
					}
				else
				if(ch_in=='|')
					{state=3;}
				else
					{
					buffer_temp.append(ch_in);
					state=1;
					}
				break;

			case 2:
				if(ch_in>=0 && ch_in<=' ')
					{state=2;}
				else
				if(ch_in=='|')
					{
					buffer_temp.truncate();
					state=3;
					}
				else
					{
					buffer_temp.append(ch_in);
					state=1;
					}
				break;

			case 3:
				buffer_temp.append('\0');
				ret|=optionFromString(buffer_temp.begin(),option_names);
				buffer_temp.clear();
				if(ch_in>=0 && ch_in<' ')
					{state=0;}
				else
					{
					buffer_temp.append(ch_in);
					state=1;
					}
				break;
			}
		++ptr;
		}
	buffer_temp.append('\0');
	return ret|=optionFromString(buffer_temp.begin(),option_names);
	}

unsigned int Anja::optionFromString(const char* string,const char* const* option_names)
	{
	auto ptr=option_names;
	while(*ptr!=nullptr)
		{
		if(strcmp(string,*ptr)==0)
			{
			return 1<<(ptr-option_names);
			}
		++ptr;
		}
	return 0;
	}
