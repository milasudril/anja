#ifdef __WAND__
target[name[valueinput.o] type[object]]
#endif

#include "valueinput.h"
#include <cstdlib>
#include <cstdio>

void ValueInput::EventHandler::textGet(ValueInput& source,double value,TextBuffer& buffer)
	{
	sprintf(buffer.begin(),"%.3f",value);
	}

double ValueInput::EventHandler::valueGet(ValueInput& source,const char* text)
	{
	char* result;
	double v=strtod(text,&result);
	if(result==text)
		{return -1;}
	return v;
	}

double ValueInput::EventHandler::valueMap(ValueInput& source,double x) const noexcept
	{
	return x;
	}

double ValueInput::EventHandler::valueMapInverse(ValueInput& source,double y) const noexcept
	{
	return y;
	}

ValueInput::EventHandler ValueInput::s_default_handler;
