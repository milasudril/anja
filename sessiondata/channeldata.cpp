//@	{"targets":[{"name":"channeldata.o","type":"object"}]}

#include "channeldata.hpp"
#include "sessionfilerecord.hpp"
#include "../common/colorstring.hpp"
#include "../common/floatconv.hpp"

#include <cstring>

using namespace Anja;

void ChannelData::clear()
	{
	m_label.clear();
	m_color=COLORS[ColorID::BLACK];
	m_state_flags=0;
	}

ChannelData::ChannelData(const SessionFileRecord& record):
	m_label(""),m_color{COLORS[ColorID::BLACK]}
	,m_state_flags(0)
	{
	auto value=record.propertyGet(String("Label"));
	if(value!=nullptr)
		{m_label=*value;}

	value=record.propertyGet(String("Color"));
	if(value!=nullptr)
		{m_color=colorFromString(value->begin());}

	dirtyClear();
//	TODO Store other data not interpreted by Anja
	}

void ChannelData::dataGet(SessionFileRecord& record) const
	{
	record.propertySet(String("Label"),m_label);

	record.propertySet(String("Color")
		,String((ColorString(m_color).begin())));
//	TODO Save other data not interpreted by Anja
	}

void ChannelData::colorSet(const ColorRGBA& color) noexcept
	{
	if(std::abs(color.red - m_color.red) > 1e-3f
		|| std::abs(color.green - m_color.green) > 1e-3f
		|| std::abs(color.blue - m_color.blue) > 1e-3f
		|| std::abs(color.alpha - m_color.alpha) > 1e-3f)
		{
		m_color=color;
		m_state_flags|=DIRTY;
		}
	}

