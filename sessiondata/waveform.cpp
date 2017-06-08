//@	 {"targets":[{"name":"waveform.o","type":"object"}]}

#include "waveform.hpp"
#include "optionstring.hpp"
#include "wavefilereader.hpp"
#include "wavefileinfo.hpp"
#include "wavefilewriter.hpp"
#include "sessionfilerecord.hpp"
#include "../common/arraysimple.hpp"
#include "../common/floatconv.hpp"
#include "../common/localeguard.hpp"

using namespace Anja;

static constexpr unsigned int BUFFER_SIZE=1024;

static constexpr const char* FLAG_NAMES[]=
	{"Loop","Sustain","Readonly","Set gain on loop",nullptr};

const char* const* Waveform::flagNames() noexcept
	{return FLAG_NAMES;}

Waveform::Waveform(const SessionFileRecord& record,const char* filename)
	{
	clear();
	fileLoad(filename);
	auto value=record.propertyGet(String("Gain/dB"));
	if(value!=nullptr)
		{gainSet(convert(value->begin()));}

	value=record.propertyGet(String("Gain random level/dB"));
	if(value!=nullptr)
		{gainRandomSet(convert(value->begin()));}

	m_flags=0;
	value=record.propertyGet(String("Options"));
	if(value!=nullptr)
		{flagsSet(optionsFromString(value->begin(),FLAG_NAMES));}

	value=record.propertyGet(String("Channel"));
	if(value!=nullptr)
		{
		long ch;
			{
			LocaleGuard locale("C");
			ch=atol(value->begin());
			}
		if(ch<1 || ch>16)
			{throw Error("A sound effect must be mapped to a channel number between 1 to 16 inclusive");}
		channelSet(ch-1);
		}

	value=record.propertyGet(String("Begin position/frames"));
	if(value==nullptr)
		{offset<Cursor::BEGIN>(0);}
	else
		{
		LocaleGuard locale("C");
		auto pos=atol(value->begin());
		offset<Cursor::BEGIN>(static_cast<int32_t>( pos) );
		}

	value=record.propertyGet(String("End position/frames"));
	if(value==nullptr)
		{offset<Cursor::END>(static_cast<int32_t>(m_data.length()-1) );}
	else
		{
		LocaleGuard locale("C");
		auto pos=atol(value->begin());
		offset<Cursor::END>( static_cast<int32_t>(pos) );
		}

	value=record.propertyGet(String("Loop begin position/frames"));
	if(value==nullptr)
		{offset<Cursor::BEGIN_LOOP>(offset<Cursor::BEGIN>());}
	else
		{
		LocaleGuard locale("C");
		auto pos=atol(value->begin());
		offset<Cursor::BEGIN_LOOP>( static_cast<int32_t>(pos) );
		}

	value=record.propertyGet(String("Loop end position/frames"));
	if(value==nullptr)
		{offset<Cursor::END_LOOP>(offset<Cursor::END>());}
	else
		{
		LocaleGuard locale("C");
		auto pos=atol(value->begin());
		offset<Cursor::END_LOOP>( static_cast<int32_t>(pos) );
		}


	dirtyClear();
	}

bool Waveform::loadPossible(const char* filename)
	{
	WavefileInfo wi;
	return WavefileReader::check(filename,wi);
	}

void Waveform::dataGet(SessionFileRecord& record) const
	{
	char buffer[32];
	sprintf(buffer,"%u",channelGet() + 1);
	record.propertySet(String("Channel")
		,String(buffer));
	sprintf(buffer,"%.7g",gainGet());
	record.propertySet(String("Gain/dB")
		,String(buffer));
	sprintf(buffer,"%.7g",gainRandomGet());
	record.propertySet(String("Gain random level/dB")
		,String(buffer));
	sprintf(buffer,"%u",offset<Cursor::BEGIN>());
	record.propertySet(String("Begin position/frames")
		,String(buffer));
	sprintf(buffer,"%u",offset<Cursor::END>());
	record.propertySet(String("End position/frames")
		,String(buffer));
	record.propertySet(String("Options")
		,stringFromOptions(flagsGet(),FLAG_NAMES));
	}

void Waveform::fileLoad(const char* filename)
	{
//	Stop loading new data into this slot if it is already in use by an audio
//	thread. TODO: This requires a mutex, since we will recieve input from the
//	audio loop as well (MIDI).
	if(flagsGet() & (PLAYBACK_RUNNING|RECORD_RUNNING) )
		{
		throw "The waveform loaded in the current slot is currently in use. "
			"Please wait for the waveform to be unlocked, or choose another "
			"slot.";
		}

//	If filename is empty or nullptr, clear the slot
	if(filename==nullptr || *filename=='\0')
		{
		clear();
		return;
		}

//	Try to load the file
	WavefileInfo info;
	WavefileReader reader(filename,info);
	clear();
	sampleRateSet(info.fs);
	capacitySet(info.n_frames);
	ArraySimple<float> buffer_tmp(BUFFER_SIZE);
	uint32_t n_read=0;
	do
		{
		n_read=reader.dataRead(buffer_tmp.begin(),BUFFER_SIZE);
		append(buffer_tmp.begin(),n_read);
		}
	while(n_read==BUFFER_SIZE);
	offsetsReset();

//	When a wavefile is loaded, we do not want to overwrite data by accident.
	flagsSet(Waveform::READONLY|Waveform::DIRTY);
	}

void Waveform::fileSave(const char* filename)
	{
	if(flagsGet() & (PLAYBACK_RUNNING|RECORD_RUNNING))
		{
		throw "The waveform loaded in the current slot is currently in use. "
			"Please wait for the waveform to be unlocked.";
		}

	WavefileInfo info{lengthFull(),sampleRateGet(),1};

	WavefileWriter writer(filename,info);
	auto ptr=beginFull();
	auto n=lengthFull();
	while(n!=0)
		{
		auto n_written=writer.dataWrite(ptr,std::min(BUFFER_SIZE,n));
		n-=n_written;
		ptr+=n_written;
		}
	m_flags&=~RECORDED;
	}
