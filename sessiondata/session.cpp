//@	{"targets":[{"name":"session.o","type":"object"}]}

#include "session.hpp"
#include "sessionfilereader.hpp"
#include "sessionfilewriter.hpp"
#include "sessionfilerecordimpl.hpp"
#include "../common/units.hpp"
#include "optionstring.hpp"
#include "../common/localeguard.hpp"
#include "../common/pathutils.hpp"
#include "../common/floatconv.hpp"

#include <cstring>

using namespace Anja;

static constexpr const char* FLAG_NAMES[]={"Use individual ports for each channel",nullptr};

const char* const* Session::flagNames() noexcept
	{return FLAG_NAMES;}

Session::Session(const char* filename):m_slot_active(0)
	{
	clear();
	SessionFileReader reader(filename);
	SessionFileRecordImpl record;

	if(!reader.recordNextGet(record))
		{throw "Invalid session file";}

	if(record.sectionLevelGet()!=0)
		{throw "Invalid session file";}

//	Get data from file header
		{
		m_filename=String(filename);
		m_directory=parentDirectory(realpath(m_filename));
		titleSet(record.titleGet());
		auto slot_num_str=record.propertyGet(String("Active slot"));
		if(slot_num_str!=nullptr)
			{
			LocaleGuard locale("C");
			auto slot_num=atol(slot_num_str->begin());
			if(slot_num<1 || slot_num>128)
				{throw "Invalid slot number";}
			slotActiveSet(slot_num-1);
			}

		auto value=record.propertyGet(String("Description"));
		if(value!=nullptr)
			{descriptionSet(*value);}

		value=record.propertyGet(String("Master gain/dB"));
		if(value!=nullptr)
			{gainSet(convert(value->begin()));}

		value=record.propertyGet(String("Options"));
		if(value!=nullptr)
			{
			flagsSet(optionsFromString(value->begin(),FLAG_NAMES));
			}
	//	TODO Store other data not interpreted by Anja
		}

//	Read records
	while(reader.recordNextGet(record))
		{
		if(record.sectionLevelGet()==0)
			{break;}

		auto title_ptr=record.titleGet().begin();
		if(strncmp(title_ptr,"Slot ",5)==0)
			{
			title_ptr+=5;
			long slot_num;
				{
				LocaleGuard locale("C");
				slot_num=atol(title_ptr);
				}
			if(slot_num<1 || slot_num>128)
				{throw "The slot number has to be between 1 and 128 inclusive";}
			--slot_num;

			WaveformView(m_waveforms[slot_num],m_waveform_data[slot_num])
				.load(record,m_directory);
			}
		else
		if(strncmp(title_ptr,"Channel ",8)==0)
			{
			title_ptr+=8;
			int ch;
				{
				LocaleGuard locale("C");
				ch=atol(title_ptr);
				}
			if(ch<1 || ch>16)
				{throw "The channel number has to be between 1 and 16 inclusive";}
			--ch;
			m_channel_data[ch]=ChannelData{record};
			m_channels[ch]=Channel{record};
			}
		}
	m_state_flags=0;
	}

void Session::waveformsClear()
	{
	//	Reset waveform data
		{
		auto ptr=m_waveform_data.begin();
		auto ptr_end=m_waveform_data.end();
		while(ptr!=ptr_end)
			{
			ptr->clear();
			++ptr;
			}
		}

		slotActiveSet(0);
	}

void Session::channelsClear()
	{
	//	Reset channel data
		{
		auto ptr=m_channel_data.begin();
		auto ptr_end=m_channel_data.end();
		auto k=0;
		while(ptr!=ptr_end)
			{
			ptr->clear();
			char buffer[16];
			sprintf(buffer,"Ch %d",k);
			ptr->labelSet(String(buffer));
			++k;
			++ptr;
			}
		}
	}

void Session::clear()
	{
	waveformsClear();
	channelsClear();
	m_filename.clear();
	m_filename.append('\0');
	m_title=String("New session");
	m_description.clear();
	m_description.append('\0');
	gainSet(-6);
	memcpy(m_color_presets.begin(),COLORS
		,std::min(int(ColorID::COLOR_END),64)*sizeof(ColorRGBA));
	m_state_flags=0;
	m_flags=0;
	}

void Session::save(const char* filename)
	{
	char buffer[32];
	SessionFileWriter writer(filename);
	SessionFileRecordImpl record_out;
	auto dir=parentDirectory(realpath(String(filename)));
	record_out.sectionLevelSet(0);
	record_out.sectionTitleSet(m_title);
	sprintf(buffer,"%u",m_slot_active + 1);
	record_out.propertySet(String("Active slot")
		,String(buffer));
	record_out.propertySet(String("Description")
		,m_description);

	sprintf(buffer,"%.7g",gainGet());
	record_out.propertySet(String("Master gain/dB")
		,String(buffer));
	record_out.propertySet(String("Options")
		,stringFromOptions(flagsGet(),FLAG_NAMES));

//	TODO Save other data not interpreted by Anja
	writer.recordWrite(record_out);
	record_out.clear();

	//	Loop through all waveform slots
		{
		auto waveform=m_waveform_data.begin();
		auto waveforms_end=m_waveform_data.end();
		auto k=0;
		while(waveform!=waveforms_end)
			{
			record_out.clear();
			record_out.sectionLevelSet(1);
			sprintf(buffer,"Slot %u",k+1);
			record_out.sectionTitleSet(String(buffer));

			WaveformView wv(m_waveforms[k],*waveform);
			wv.store(record_out,dir);
			writer.recordWrite(record_out);
			wv.dirtyClear();
			++waveform;
			++k;
			}
		}

	//	Loop through all channels
		{
		auto channel=m_channel_data.begin();
		auto channels_end=m_channel_data.end();
		auto k=0;
		while(channel!=channels_end)
			{
			record_out.clear();
			record_out.sectionLevelSet(1);
			sprintf(buffer,"Channel %u",k+1);
			record_out.sectionTitleSet(String(buffer));
			channel->dataGet(record_out);
			m_channels[k].dataGet(record_out);
			writer.recordWrite(record_out);
			++k;
			++channel;
			}
		}
	m_directory=dir;
	dirtyClear();
	}

bool Session::dirtyIs() const noexcept
	{
	if(m_state_flags&SESSION_DIRTY)
		{return 1;}

	//	Check if waveform data is dirty
		{
		auto ptr_wfd=m_waveform_data.begin();
		auto ptr_end=m_waveform_data.end();
		int k=0;
		while(ptr_wfd!=ptr_end)
			{
			if(ptr_wfd->dirtyIs() || m_waveforms[k].dirtyIs())
				{return 1;}
			++ptr_wfd;
			++k;
			}
		}

	//	Check if channel data is dirty
		{
		auto ptr_channel=m_channel_data.begin();
		auto ptr_end=m_channel_data.end();
		int k=0;
		while(ptr_channel!=ptr_end)
			{
			if(ptr_channel->dirtyIs() ||  m_channels[k].dirtyIs())
				{return 1;}
			++ptr_channel;
			++k;
			}
		}

	return 0;
	}
