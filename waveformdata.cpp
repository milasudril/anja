#ifdef __WAND__
target[name[waveformdata.o] type[object]]
#endif

#include "waveformdata.h"
#include "sessionfilerecord.h"

WaveformData::WaveformData(const SessionFileRecord& record
	,const ArrayDynamicShort<char>& load_path):
	m_description(""),m_key_label(""),m_color(0.25f,0.0f,.5f,1.0f)
	,m_waveform_range(nullptr,0,0)
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

void WaveformData::fileLoad(const ArrayDynamicShort<char>& filename
	,const ArrayDynamicShort<char>& load_path)
	{

	m_filename=filename;
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
