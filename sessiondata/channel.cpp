//@	{"targets":[{"name":"channel.o","type":"object"}]}

#include "channel.hpp"
#include "sessionfilerecord.hpp"
#include "../common/string.hpp"
#include "../common/floatconv.hpp"

using namespace Anja;

Channel::Channel(const SessionFileRecord& record)
	{
	valuesInit();

	auto value=record.propertyGet(String("Gain/dB"));
	if(value!=nullptr)
		{gain(convert(value->begin()));}

	value=record.propertyGet(String("Fade time/s"));
	if(value!=nullptr)
		{fadeTime(convert(value->begin()));}
	dirtyClear();
	}

const Channel& Channel::store(SessionFileRecord& record) const
	{
	char buffer[32];
	sprintf(buffer,"%.7g",gain());
	record.propertySet(String("Gain/dB"),String(buffer));
	sprintf(buffer,"%.7g",fadeTime());
	record.propertySet(String("Fade time/s"),String(buffer));

	return *this;
	}
