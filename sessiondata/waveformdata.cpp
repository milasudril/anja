//@	 {"targets":[{"name":"waveformdata.o","type":"object"}]}

#include "waveformdata.hpp"
#include "sessionfilerecord.hpp"
#include "optionstring.hpp"
#include "../common/colorstring.hpp"
#include "../common/pathutils.hpp"
#include "../common/floatconv.hpp"
#include "../common/localeguard.hpp"

#include <cstdlib>
#include <cstring>

using namespace Anja;

WaveformData::WaveformData(const SessionFileRecord& record
	,const ArrayDynamicShort<char>& load_path):m_filename(""),m_description("")
	,m_key_label(""),m_color(0.25f,0.0f,.5f,1.0f),m_stateflags(0)
	{
	auto value=record.propertyGet(ArrayDynamicShort<char>("Filename"));
	if(value!=nullptr)
		{filenameSet(*value,load_path);}

	value=record.propertyGet(ArrayDynamicShort<char>("Description"));
	if(value!=nullptr)
		{descriptionSet(*value);}

	value=record.propertyGet(ArrayDynamicShort<char>("Color"));
	if(value!=nullptr)
		{keyColorSet(colorFromString(value->begin()));}

/*	TODO: Move these to Waveform class

	value=record.propertyGet("Playback gain/dB");
	if(value!=nullptr)
		{r_waveform->gainSet(convert(value->begin()));}

	value=record.propertyGet("Playback gain random level/dB");
	if(value!=nullptr)
		{r_waveform->gainRandomSet(convert(value->begin()));}

	value=record.propertyGet("Options");
	if(value!=nullptr)
		{
		r_waveform->flagsSet(optionsFromString(value->begin(),Waveform::FLAG_NAMES));
		}

	value=record.propertyGet("Playback channel");
	if(value!=nullptr)
		{
		long ch;
			{
			LocaleGuard locale("C");
			ch=atol(value->begin());
			}
		if(ch<1 || ch>16)
			{
			throw "A sound effect must be mapped to a channel number between 1 to 16 inclusive";
			}
		r_waveform->channelSet(ch-1);
		}

	value=record.propertyGet("Playback starting position/frames");
	if(value!=nullptr)
		{
		LocaleGuard locale("C");
		auto pos=atol(value->begin());
		r_waveform->offsetBeginSet(pos);
		}

	value=record.propertyGet("Playback end position/frames");
	if(value!=nullptr)
		{
		LocaleGuard locale("C");
		auto pos=atol(value->begin());
		r_waveform->offsetEndSet(pos);
		}*/
//	TODO Store other data not interpreted by Anja

	dirtyClear();
	}

void WaveformData::clear()
	{
	m_filename.clear();
	m_filename.append('\0');
	m_description.clear();
	m_description.append('\0');
	m_color=COLORS[ColorID::BLACK];
	m_stateflags=0;
	}

void WaveformData::dataGet(SessionFileRecord& record
	,const ArrayDynamicShort<char>& load_path) const
	{
	record.clear();

	auto filename_out=m_filename;
	if(*filename_out.begin()!='\0')
		{
		filename_out=makeRelativeTo(filename_out.begin(),load_path.begin());
		}


	record.propertySet(ArrayDynamicShort<char>("Filename"),filename_out);
	record.propertySet(ArrayDynamicShort<char>("Description"),m_description);
	record.propertySet(ArrayDynamicShort<char>("Color"),ArrayDynamicShort<char>(ColorString(m_color).begin()));
/*
TODO: Move these to waveform class
	char buffer[32];
	sprintf(buffer,"%u",r_waveform->channelGet() + 1);
	record.propertySet("Playback channel",buffer);
	sprintf(buffer,"%.7g",r_waveform->gainGet());
	record.propertySet("Playback gain/dB",buffer);
	sprintf(buffer,"%.7g",r_waveform->gainRandomGet());
	record.propertySet("Playback gain random level/dB",buffer);
	sprintf(buffer,"%u",r_waveform->offsetBeginGet());
	record.propertySet("Playback starting position/frames",buffer);
	sprintf(buffer,"%u",r_waveform->offsetEndGet());
	record.propertySet("Playback end position/frames",buffer);
	record.propertySet("Options",stringFromOptions(r_waveform->flagsGet()
		,Waveform::FLAG_NAMES));
*/
//	TODO Save other data not interpreted by Anja
	}

void WaveformData::filenameSet(const char* filename)
	{
	m_filename=ArrayDynamicShort<char>(filename);
	m_stateflags|=DIRTY;
	}

void WaveformData::filenameSet(const ArrayDynamicShort<char>& filename
	,const ArrayDynamicShort<char>& load_path)
	{
	if(absoluteIs(filename))
		{filenameSet(filename.begin());}
	else
	if(*(filename.begin())!='\0')
		{
		auto fullpath=load_path;
		fullpath.truncate().append(filename).append('\0');
		filenameSet(fullpath.begin());
		}
	}

void WaveformData::descriptionSet(const char* description)
	{
	if(strcmp(m_description.begin(),description)==0)
		{return;}
	m_description=ArrayDynamicShort<char>(description);
	m_key_label.clear();
	auto state=0;
	auto ptr=description;
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
		auto ptr=description;
		while(*ptr!=' ' && *ptr!='\0')
			{
			m_key_label.append(*ptr);
			++ptr;
			}
		}
	m_key_label.append('\0');
	m_stateflags|=DIRTY;
	}

void WaveformData::descriptionSet(const ArrayDynamicShort<char>& description)
	{
	descriptionSet(description.begin());
	}


void WaveformData::keyColorSet(const ColorRGBA& color)
	{
	m_color=color;
	m_stateflags|=DIRTY;
	}
