#ifdef __WAND__
target[name[waveform.o] type[object]]
#endif

#include "waveform.h"
#include <cstring>

const char* Waveform::FLAG_NAMES[]=
	{"Loop","Sustain","Readonly",nullptr};

Waveform& Waveform::flagsSet(const ArrayDynamicShort<char>& flags)
	{
	auto ptr=flags.begin();
	auto ptr_end=flags.end();
	ArrayDynamicShort<char> buffer_temp;
	int state=0;
	while(ptr!=ptr_end)
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
				flagSet(buffer_temp);
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
	return flagSet(buffer_temp);
	}

Waveform& Waveform::flagSet(const ArrayDynamicShort<char>& flagname)
	{
	auto ptr=FLAG_NAMES;

	while(*ptr!=nullptr)
		{
		if(strcmp(flagname.begin(),*ptr)==0)
			{
			flagsSet(1<<(ptr-FLAG_NAMES));
			break;
			}
		++ptr;
		}
	return *this;
	}
