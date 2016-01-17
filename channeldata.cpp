#ifdef __WAND__
target[name[channeldata.o] type[object]]
#endif

#include "channeldata.h"
#include "channel.h"
#include "sessionfilerecord.h"
#include "framework/floatconv.h"

ChannelData::ChannelData():m_label(""),r_channel(nullptr)
	{}

ChannelData::ChannelData(const SessionFileRecord& record,Channel& channel):
	m_label(""),r_channel(&channel)
	{
	r_channel->valuesInit();

	auto value=record.propertyGet("Label");
	if(value!=nullptr)
		{m_label=*value;}

	value=record.propertyGet("Gain/dB");
	if(value!=nullptr)
		{r_channel->gainSet(convert(value->begin()));}

	value=record.propertyGet("Fade time/s");
	if(value!=nullptr)
		{r_channel->fadeTimeSet(convert(value->begin()));}
	}
