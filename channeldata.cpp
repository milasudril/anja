#ifdef __WAND__
target[name[channeldata.o] type[object]]
#endif

#include "channeldata.h"
#include "channel.h"
#include "sessionfilerecord.h"
#include "colorstring.h"
#include "framework/floatconv.h"

ChannelData::ChannelData():m_label(""),m_color(COLORS[ColorID::BLACK])
	,r_channel(nullptr),r_key(nullptr)
	{}

ChannelData::ChannelData(const SessionFileRecord& record,Channel& channel
	,KeyboardLayout::KeyDescriptor* key):
	m_label(""),m_color{0.25f,0.0f,0.5f,1.0f},r_channel(&channel),r_key(key)
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

	value=record.propertyGet("Color");
	if(value!=nullptr)
		{m_color=colorFromString(value->begin());}

	if(r_key!=nullptr)
		{
		r_key->labelSet(m_label.begin());
		r_key->colorBackgroundSet(m_color);
		}
//	TODO Store other data not interpreted by Anja
	}

void ChannelData::dataGet(SessionFileRecord& record) const
	{
	record.clear();
	record.propertySet("Label",m_label);
	char buffer[32];
	sprintf(buffer,"%.7g",r_channel->gainGet());
	record.propertySet("Gain/dB",buffer);
	sprintf(buffer,"%.7g",r_channel->fadeTimeGet());
	record.propertySet("Fade time/s",buffer);
	record.propertySet("Color",ColorString(m_color).begin());
//	TODO Save other data not interpreted by Anja
	}
