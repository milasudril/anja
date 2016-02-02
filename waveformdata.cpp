#ifdef __WAND__
target[name[waveformdata.o] type[object]]
#endif

#include "waveformdata.h"
#include "sessionfilerecord.h"
#include "wavefilereader.h"
#include "wavefileinfo.h"
#include "colorstring.h"
#include "optionstring.h"
#include "framework/pathutils.h"
#include "framework/array_simple.h"
#include "framework/floatconv.h"
#include "framework/localeguard.h"

#include <cstdlib>
#include <cstring>

WaveformData::WaveformData(const SessionFileRecord& record
	,const ArrayDynamicShort<char>& load_path
	,Waveform& waveform
	,KeyboardLayout::KeyDescriptor* key):m_filename(""),m_description("")
	,m_key_label(""),m_color(0.25f,0.0f,.5f,1.0f)
	,r_key(key),r_waveform(&waveform)
	{
	r_waveform->valuesInit();

	auto value=record.propertyGet("Filename");
	if(value!=nullptr)
		{fileLoad(*value,load_path);}

	value=record.propertyGet("Description");
	if(value!=nullptr)
		{descriptionSet(*value);}

	value=record.propertyGet("Color");
	if(value!=nullptr)
		{keyColorSet(colorFromString(value->begin()));}

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
		}
//	TODO Store other data not interpreted by Anja
	}

void WaveformData::dataGet(SessionFileRecord& record) const
	{
	record.clear();
	record.propertySet("Filename",m_filename);
	record.propertySet("Description",m_description);
	record.propertySet("Color",ColorString(m_color).begin());
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

//	TODO Format flag string
//	TODO Save other data not interpreted by Anja
	}

WaveformData::WaveformData():m_filename(""),m_description("")
	,m_color{0.0f,0.0f,0.0f,1},r_key(nullptr),r_waveform(nullptr)
	{}

void WaveformData::fileLoad(const char* filename)
	{
//	Do not try to load the file if it is the same file
	if(strcmp(m_filename.begin(),filename)==0 && *filename!='\0')
		{return;}

	if(r_waveform->flagsGet() & Waveform::LOCKED)
		{
		throw "The waveform loaded in current slot is currently in use. "
			"Please wait for the waveform to be unlocked, or choose another "
			"slot.";
		}

	if(*filename=='\0')
		{
		r_waveform->clear();
		float vals[2]={1e-7f,1e-7f};
		r_waveform->append(vals,2);
		r_waveform->valuesInit();
		r_waveform->sampleRateSet(1000.0);
		m_filename=filename;
		return;
		}

	WavefileInfo info;
	auto reader=WavefileReader::create(filename,info);
	r_waveform->clear();
	r_waveform->sampleRateSet(info.fs);
	r_waveform->capacitySet(info.n_frames);
	const uint32_t buffer_size=1024;
	ArraySimple<float> buffer_tmp(buffer_size);
	uint32_t n_read=0;
	do
		{
		n_read=reader->dataRead(buffer_tmp.begin(),buffer_size);
		r_waveform->append(buffer_tmp.begin(),n_read);
		}
	while(n_read==buffer_size);
	r_waveform->offsetsReset();
	r_waveform->flagsSet(Waveform::READONLY);
	m_filename=filename;
	}

void WaveformData::fileLoad(const ArrayDynamicShort<char>& filename
	,const ArrayDynamicShort<char>& load_path)
	{
	if(absoluteIs(filename))
		{
		fileLoad(filename.begin());
		}
	else
	if(*(filename.begin())!='\0')
		{
		auto fullpath=load_path;
		fullpath.truncate().append(filename).append('\0');
		fileLoad(fullpath.begin());
		}
	}

void WaveformData::descriptionSet(const char* description)
	{
	m_description=description;
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

	if(r_key!=nullptr)
		{r_key->labelSet(m_key_label.begin());}
	}

void WaveformData::descriptionSet(const ArrayDynamicShort<char>& description)
	{
	descriptionSet(description.begin());
	}


void WaveformData::keyColorSet(const ColorRGBA& color)
	{
	if(r_key!=nullptr)
		{r_key->colorBackgroundSet(color);}
	m_color=color;
	}
