//@	{"targets":[{"name":"session.o","type":"object"}]}

#include "session.hpp"
#include "sessionfilereader.hpp"
#include "sessionfilewriter.hpp"
#include "sessionfilerecordimpl.hpp"
#include "optionstring.hpp"
#include "../common/localeguard.hpp"
#include "../common/pathutils.hpp"
#include "../common/floatconv.hpp"
#include "../common/colorstring.hpp"
#include "../common/error.hpp"

#include <cstring>

using namespace Anja;

static constexpr const char* FLAG_NAMES[]=
	{
	 "Use individual ports for each channel"
	,"Allow external MIDI sources to set playback channel"
	,nullptr
	};

const char* const* Session::flagNames() noexcept
	{return FLAG_NAMES;}

static ArrayDynamicShort<ColorRGBA> color_presets_load(const char* ptr)
	{
	ArrayDynamicShort<ColorRGBA> ret;
	String buffer;
	while(1)
		{
		auto ch_in=*ptr;
		switch(ch_in)
			{
			case '\0':
				ret.append(colorFromString(buffer.begin()));
				return std::move(ret);
			case '|':
				ret.append(colorFromString(buffer.begin()));
				buffer.clear();
				break;
			default:
				buffer.append(ch_in);
			}

		++ptr;
		}
	return std::move(ret);
	}

template<class Range>
static String string_from_color_presets(const Range& r)
	{
	String ret;
	std::for_each(r.begin(),r.end()-1,[&ret](const ColorRGBA& c)
		{
		ret.append(ColorString(c).begin()).append('|');
		});
	ret.append(ColorString(*(r.end()-1)).begin());

	return std::move(ret);
	}


Session::Session(const char* filename):m_slot_active(0)
	{
	clear();
	SessionFileReader reader(filename);
	SessionFileRecordImpl record;

	if(!reader.recordNextGet(record))
		{throw Error(filename," is not a valid session file.");}

	if(record.levelGet()!=0)
		{throw Error(filename," is not a valid session file.");}

//	Get data from file header
		{
		m_filename=String(filename);
		m_directory=parentDirectory(realpath(filename));
		titleSet(record.titleGet().begin());
		auto slot_num_str=record.propertyGet(String("Active slot"));
		if(slot_num_str!=nullptr)
			{
			LocaleGuard locale("C");
			auto slot_num=atol(slot_num_str->begin());
			if(slot_num<1 || slot_num>128)
				{throw Error(filename," contains invalid data. Slot numbers must be between 1 to 128 inclusive.");}
			slotActiveSet(slot_num-1);
			}

		auto value=record.propertyGet(String("Description"));
		if(value!=nullptr)
			{descriptionSet(value->begin());}

		value=record.propertyGet(String("Master gain/dB"));
		if(value!=nullptr)
			{gainSet(convert(value->begin()));}

		value=record.propertyGet(String("Options"));
		if(value!=nullptr)
			{
			flagsSet(optionsFromString(value->begin(),FLAG_NAMES));
			}

		value=record.propertyGet(String("Color presets"));
		if(value!=nullptr)
			{
			m_color_presets=color_presets_load(value->begin());
			}
	//	TODO Store other data not interpreted by Anja
		}

//	Read records
	while(reader.recordNextGet(record))
		{
		if(record.levelGet()==0)
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
				{throw Error(filename," contains invalid data. Slot numbers must be between 1 to 128 inclusive.");}
			--slot_num;

			WaveformProxy(m_waveforms[slot_num],m_waveform_data[slot_num],m_directory
				,slot_num).load(record);
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
				{throw Error(filename," contains invalid data. Channel numbers must be between 1 to 16 inclusive.");}
			--ch;
			ChannelProxy(m_channels[ch],m_channel_data[ch]).load(record);
			}
		}
	dirtyClear();
	}

void Session::waveformsClear()
	{
	std::for_each(m_waveforms.begin(),m_waveforms.end(),[](Waveform& wf)
		{wf.clear();});

	std::for_each(m_waveform_data.begin(),m_waveform_data.end(),[](WaveformData& wd)
		{wd.clear();});

	slotActiveSet(0);
	}

ArraySimple<String> Session::channelLabelsGet() const
	{
	ArraySimple<String> ret(m_channel_data.length());
	auto k=0;
	std::for_each(m_channel_data.begin(),m_channel_data.end(),[&ret,&k](const auto& x)
		{
		ret[k]=x.label();
		++k;
		});
	return std::move(ret);
	}

ArraySimple<ColorRGBA> Session::channelColorsGet() const
	{
	ArraySimple<ColorRGBA> ret(m_channel_data.length());
	auto k=0;
	std::for_each(m_channel_data.begin(),m_channel_data.end(),[&ret,&k](const auto& x)
		{
		ret[k]=x.color();
		++k;
		});
	return std::move(ret);
	}


void Session::channelsClear()
	{
	auto k=0;
	std::for_each(m_channel_data.begin(),m_channel_data.end(),[&k](auto& x)
		{
		char buffer[16];
		sprintf(buffer,"Ch %d",k);
		x.clear().label(String(buffer)).dirtyClear();
		++k;
		});

	std::for_each(m_channels.begin(),m_channels.end(),[](auto& x)
		{x.gain(0.0f).fadeTime(1e-3f).dirtyClear();});
	}

Session& Session::colorPresetsSet(const ColorRGBA* begin,const ColorRGBA* end)
	{
	m_color_presets.clear();
	std::for_each(begin,end,[this](const ColorRGBA& c)
		{m_color_presets.append(c);});
	m_state_flags|=SESSION_DIRTY;
	return *this;
	}


void Session::clear()
	{
	waveformsClear();
	channelsClear();
	m_filename.clear();
	m_title=String("New session");
	m_description.clear();
	m_gain=-6;
	m_color_presets.clear();
	std::for_each(COLORS,COLORS + ColorID::COLOR_END,[this](const ColorRGBA& x)
		{m_color_presets.append(x);});
	m_state_flags=0;
	m_flags=0;
	}

bool Session::loadPossible(const char* filename) const
	{
	return SessionFileReader::check(filename);
	}

void Session::save(const char* filename)
	{
	char buffer[32];
	SessionFileRecordImpl record_out;
	auto dir=parentDirectory(realpath(filename));
	record_out.levelSet(0);
	record_out.titleSet(m_title);
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
	if(m_color_presets.length()!=0)
		{
		record_out.propertySet(String("Color presets")
			,string_from_color_presets(m_color_presets));
		}

	SessionFileWriter writer(filename);
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
			record_out.levelSet(1);
			sprintf(buffer,"Slot %u",k+1);
			record_out.titleSet(String(buffer));

			WaveformProxy wv(m_waveforms[k],*waveform,dir,k);
			wv.store(record_out);
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
			record_out.levelSet(1);
			sprintf(buffer,"Channel %u",k+1);
			record_out.titleSet(String(buffer));
			ChannelProxy cv(m_channels[k],*channel);
			cv.store(record_out);
			writer.recordWrite(record_out);
			cv.dirtyClear();
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
			if(ptr_wfd->dirty() || m_waveforms[k].dirty())
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
			if(ptr_channel->dirty() ||  m_channels[k].dirty())
				{return 1;}
			++ptr_channel;
			++k;
			}
		}

	return 0;
	}
