#ifdef __WAND__
target[name[waveformdata.o] type[object]]
#endif

#include "waveformdata.h"
#include "sessionfilerecord.h"
#include "wavefilereader.h"
#include "wavefileinfo.h"
#include "framework/array_simple.h"

WaveformData::WaveformData(const SessionFileRecord& record
	,const ArrayDynamicShort<char>& load_path
	,WaveformStorage& storage):
	m_description(""),m_key_label(""),m_color(0.25f,0.0f,.5f,1.0f)
	,m_waveform_range(nullptr,0,0),r_waveform(&storage)
	{
	auto value=record.propertyGet("Filename");
	if(value==nullptr)
		{throw "Filename is mandatory";}

	fileLoad(*value,load_path);

	value=record.propertyGet("Description");
	if(value!=nullptr)
		{
		descriptionSet(*value);
		}
	}

WaveformData::WaveformData():m_filename(""),m_description("")
	,m_color{0.0f,0.0f,0.0f,1},m_waveform_range(nullptr,0,0)
	{}

void WaveformData::fileLoad(const ArrayDynamicShort<char>& filename
	,const ArrayDynamicShort<char>& load_path)
	{
	m_filename=filename;
	auto fullpath=load_path;
	fullpath.truncate().append(filename).append('\0');
	WavefileInfo info;
	auto reader=WavefileReader::create(fullpath.begin(),info);

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
	m_waveform_range=*r_waveform;
	}

void WaveformData::descriptionSet(const ArrayDynamicShort<char>& description)
	{
	m_description=description;
	m_key_label.clear();
	auto ptr=description.begin();
	auto ptr_end=description.end();
	auto state=0;
	while(ptr!=ptr_end)
		{
		switch(*ptr)
			{
			case '[':
				state=1;
				break;
			case ']':
				ptr=ptr_end-1;
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
	m_key_label.append('\0');
	}
