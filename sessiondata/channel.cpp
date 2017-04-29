//@	{"targets":[{"name":"channel.o","type":"object"}]}

#include "channel.hpp"
#include "sessionfilerecord.hpp"
#include "../common/arraydynamicshort.hpp"
#include "../common/floatconv.hpp"

using namespace Anja;

Channel::Channel(const SessionFileRecord& record)
	{
	valuesInit();

	auto value=record.propertyGet(String("Gain/dB"));
	if(value!=nullptr)
		{gainSet(convert(value->begin()));}

	value=record.propertyGet(String("Fade time/s"));
	if(value!=nullptr)
		{fadeTimeSet(convert(value->begin()));}
	}

void Channel::dataGet(SessionFileRecord& record) const
	{
	char buffer[32];
	sprintf(buffer,"%.7g",gainGet());
	record.propertySet(String("Gain/dB"),String(buffer));
	sprintf(buffer,"%.7g",fadeTimeGet());
	record.propertySet(String("Fade time/s"),String(buffer));
	}
