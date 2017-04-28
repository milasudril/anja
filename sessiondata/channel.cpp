//@	{"targets":[{"name":"channel.o","type":"object"}]}

#include "channel.hpp"
#include "sessionfilerecord.hpp"
#include "../common/arraydynamicshort.hpp"
#include "../common/floatconv.hpp"

using namespace Anja;

Channel::Channel(const SessionFileRecord& record)
	{
	auto value=record.propertyGet(ArrayDynamicShort<char>("Gain/dB"));
	if(value!=nullptr)
		{gainSet(convert(value->begin()));}

	value=record.propertyGet(ArrayDynamicShort<char>("Fade time/s"));
	if(value!=nullptr)
		{fadeTimeSet(convert(value->begin()));}
	}

void Channel::dataGet(SessionFileRecord& record) const
	{
	char buffer[32];
	sprintf(buffer,"%.7g",gainGet());
	record.propertySet(ArrayDynamicShort<char>("Gain/dB"),ArrayDynamicShort<char>(buffer));
	sprintf(buffer,"%.7g",fadeTimeGet());
	record.propertySet(ArrayDynamicShort<char>("Fade time/s"),ArrayDynamicShort<char>(buffer));
	}
