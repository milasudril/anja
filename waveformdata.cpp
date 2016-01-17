#ifdef __WAND__
target[name[waveformdata.o] type[object]]
#endif

#include "waveformdata.h"
#include "sessionfilerecord.h"
#include "wavefilereader.h"
#include "wavefileinfo.h"
#include "framework/pathutils.h"
#include "framework/array_simple.h"
#include "framework/floatconv.h"

#include <cstring>

WaveformData::WaveformData(const SessionFileRecord& record
	,const ArrayDynamicShort<char>& load_path
	,Waveform& waveform
	,KeyboardLayout::KeyDescriptor& key):m_filename(""),m_description("")
	,m_key_label(""),m_color(0.25f,0.0f,.5f,1.0f)
	,r_key(&key),r_waveform(&waveform)
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
		{keyColorSet(value->begin());}

	value=record.propertyGet("Playback gain/dB");
	if(value!=nullptr)
		{r_waveform->gainSet(convert(value->begin()));}

	value=record.propertyGet("Playback gain random level/dB");
	if(value!=nullptr)
		{r_waveform->gainRandomSet(convert(value->begin()));}

	value=record.propertyGet("Options");
	if(value!=nullptr)
		{r_waveform->flagsSet(*value);}
	}

WaveformData::WaveformData():m_filename(""),m_description("")
	,m_color{0.0f,0.0f,0.0f,1},r_key(nullptr),r_waveform(nullptr)
	{}

void WaveformData::fileLoad(const char* filename)
	{
	if(strcmp(m_filename.begin(),filename)==0)
		{return;}
	if(r_waveform->flagsGet() & Waveform::LOCKED)
		{
		throw "The waveform loaded in current slot is currently in use. "
			"Please wait for the waveform to be unlocked, or choose another "
			"slot.";
		}
	WavefileInfo info;
	printf("%s\n",filename);
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

void WaveformData::keyColorSet(const char* colorstring)
	{
	ArrayDynamicShort<char> buffer;
	float values[4]={0.0f,0.0f,0.0f,1.0f};
	uint8_t count=0;
	while(*colorstring!='\0' && count!=4)
		{
		switch(*colorstring)
			{
			case ';':
				buffer.append('\0');
				values[count]=convert(buffer.begin());
				++count;
				buffer.clear();
				break;
			default:
				buffer.append(*colorstring);
				break;
			}
		++colorstring;
		}
	if(count!=4)
		{
		buffer.append('\0');
		values[count]=convert(buffer.begin());
		}
	keyColorSet({values[0],values[1],values[2],values[3]});
	}

void WaveformData::keyColorSet(const ColorRGBA& color)
	{
	if(r_key!=nullptr)
		{r_key->colorBackgroundSet(color);}
	m_color=color;
	}

void WaveformData::keyColorGet(ColorString& buffer)
	{
	sprintf(buffer.begin(),"%.7g; %.7g; %.7g; %.7g"
		,m_color.red,m_color.green,m_color.blue,m_color.alpha);
	}
