//@	 {"targets":[{"name":"waveformdata.o","type":"object"}]}

#include "waveformdata.hpp"
#include "sessionfilerecord.hpp"
#include "optionstring.hpp"
#include "../common/colorstring.hpp"
#include "../common/pathutils.hpp"

#include <cstdlib>
#include <cstring>

using namespace Anja;

WaveformData::WaveformData(const SessionFileRecord& record):m_filename(""),m_description("")
	,m_key_label(""),m_color(0.25f,0.0f,.5f,1.0f),m_stateflags(0)
	{
	auto value=record.propertyGet(String("Description"));
	if(value!=nullptr)
		{descriptionSet(value->begin());}

	value=record.propertyGet(String("Color"));
	if(value!=nullptr)
		{keyColorSet(colorFromString(value->begin()));}


	value=record.propertyGet(String("Filename"));
	if(value!=nullptr)
		{m_filename=*value;}

	dirtyClear();
	}

void WaveformData::clear()
	{
	m_filename.clear();
	m_description.clear();
	m_key_label.clear();
	m_color=COLORS[ColorID::BLACK];
	m_stateflags=0;
	}

void WaveformData::dataGet(SessionFileRecord& record) const
	{
	record.propertySet(String("Description"),m_description);
	record.propertySet(String("Color"),String(ColorString(m_color).begin()));

//	TODO Save other data not interpreted by Anja
	}

void WaveformData::key_label_update()
	{
	m_key_label.clear();
	auto state=0;
	auto ptr=m_description.begin();
	while(*ptr!='\0' && *ptr!=']')
		{
		switch(*ptr)
			{
			case '[':
				state=1;
				break;
			default:
				switch(state)
					{
					case 0:
						break;
					case 1:
						m_key_label.append(*ptr);
						break;
					}
			}
		++ptr;
		}

	if(m_key_label.length()==0)
		{
		auto ptr=m_description.begin();
		while(*ptr!=' ' && *ptr!='\0')
			{
			m_key_label.append(*ptr);
			++ptr;
			}
		}
	m_stateflags|=DIRTY;
	}

void WaveformData::keyColorSet(const ColorRGBA& color)
	{
	m_color=color;
	m_stateflags|=DIRTY;
	}
