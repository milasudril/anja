#ifdef __WAND__
target[name[session.o] type[object]]
#endif

#include "session.h"
#include "sessionfilereader.h"
#include "sessionfilewriter.h"
#include "sessionfilerecordimpl.h"
#include "audioconnection.h"
#include "units.h"
#include "optionstring.h"
#include "framework/localeguard.h"
#include "framework/pathutils.h"
#include "framework/floatconv.h"
#include "midiconstants/statuscodes.h"
#include "midiconstants/controlcodes.h"

#include <cstring>

#include <unistd.h>

const char* Session::FLAG_NAMES[]={"Use individual ports for each channel",nullptr};

void Session::waveformsClear()
	{
	//	Reset slot scancodes
		{
		memset(m_scancode_to_slot.begin(),-1,sizeof(m_scancode_to_slot));
		memset(m_slot_to_scancode.begin(),0,sizeof(m_slot_to_scancode));
		auto ptr=m_keyboard.typingAreaScancodesBegin();
		auto ptr_end=m_keyboard.typingAreaScancodesEnd();
		uint8_t scancode_prev=0;
		uint8_t k=0;
		while(ptr!=ptr_end)
			{
			auto val=*ptr;
			if(val!=scancode_prev && val!=0)
				{
				m_scancode_to_slot[val]=k;
				m_slot_to_scancode[k]=val;
				++k;
				}
			scancode_prev=val;
			++ptr;
			}
		}

	//	Reset MIDI keys
		{
		memset(m_midikey_to_slot.begin(),-1,sizeof(m_midikey_to_slot));
		for(uint8_t k=36;k < 128 + 36;++k)
			{
			m_midikey_to_slot[k%128]=(k-36)%128;
			}
		}


	//	Reset waveform data
		{
		auto ptr=m_waveform_data.begin();
		auto ptr_end=m_waveform_data.end();
		auto ptr_waveform=m_waveforms.begin();
		auto slot=0;
		auto& keyboard=m_keyboard;
		while(ptr!=ptr_end)
			{
			ptr->waveformSet(*ptr_waveform);
			auto scancode=m_slot_to_scancode[slot];
			ptr->keySet(scancode==0?nullptr:keyboard.keyFromScancode(scancode));
			ptr->fileLoad("");
			ptr->descriptionSet("");
			ptr->keyColorSet(COLORS[ColorID::BLACK]);
			++slot;
			++ptr;
			++ptr_waveform;
			}
		}

		slotActiveSet(0);
	}

void Session::channelsClear()
	{
	//	Reset channel scancodes
		{
		memset(m_channel_to_scancode.begin(),0,sizeof(m_channel_to_scancode));
		memset(m_scancode_to_channel.begin(),-1,sizeof(m_scancode_to_channel));
		auto ptr=m_keyboard.functionKeysScancodesBegin();
		auto ptr_end=m_keyboard.functionKeysScancodesEnd();
		uint8_t scancode_prev=0;
		uint8_t k=0;
		while(ptr!=ptr_end)
			{
			auto val=*ptr;
			if(val!=scancode_prev && val!=0)
				{
				m_scancode_to_channel[val]=k;
				m_channel_to_scancode[k]=val;
				++k;
				}
			scancode_prev=val;
			++ptr;
			}
		}

	//	Reset channels
		{
		auto ptr=m_channels.begin();
		auto ptr_end=m_channels.end();
		while(ptr!=ptr_end)
			{
			ptr->valuesInit();
			++ptr;
			}
		}

	//	Reset channel data
		{
		auto ptr=m_channel_data.begin();
		auto ptr_channel=m_channels.begin();
		auto ptr_end=m_channel_data.end();
		auto k=0;
		while(ptr!=ptr_end)
			{
			char buffer[16];
			sprintf(buffer,"Ch %u",k+1);
			auto scancode=m_channel_to_scancode[k];
			ptr->keySet(scancode==0?nullptr:m_keyboard.keyFromScancode(scancode));
			ptr->labelSet(buffer);
			ptr->channelSet(*ptr_channel);
			ptr->colorSet(COLORS[ColorID::BLACK]);
			++ptr_channel;
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
	m_title="New session";
	m_description.clear();
	m_description.append('\0');
	masterGainSet(-6);
	memcpy(m_color_presets.begin(),COLORS
		,std::min(int(ColorID::COLOR_END),64)*sizeof(ColorRGBA));
	m_state_flags=0;
	m_flags=0;
	}

void Session::load(const char* filename)
	{
	auto reader=SessionFileReader::create(filename);
	SessionFileRecordImpl record;

	if(!reader->recordNextGet(record))
		{throw "Invalid session file";}

	if(record.sectionLevelGet()!=0)
		{throw "Invalid session file";}

	clear();

//	Get data from file header
		{
		m_filename=filename;
		m_directory=parentDirectory(realpath(m_filename));
		titleSet(record.titleGet());
		auto slot_num_str=record.propertyGet("Active slot");
		if(slot_num_str!=nullptr)
			{
			LocaleGuard locale("C");
			auto slot_num=atol(slot_num_str->begin());
			if(slot_num<1 || slot_num>128)
				{throw "Invalid slot number";}
			slotActiveSet(slot_num-1);
			}

		auto value=record.propertyGet("Description");
		if(value!=nullptr)
			{descriptionSet(*value);}

		value=record.propertyGet("Master gain/dB");
		if(value!=nullptr)
			{masterGainSet(convert(value->begin()));}

		value=record.propertyGet("Options");
		if(value!=nullptr)
			{
			flagsSet(optionsFromString(value->begin(),FLAG_NAMES));
			}
	//	TODO Store other data not interpreted by Anja
		}

//	Read records
	while(reader->recordNextGet(record))
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

			auto scancode=m_slot_to_scancode[slot_num];
			auto key=(scancode==0)?
				nullptr:m_keyboard.keyFromScancode(scancode);

			m_waveform_data[slot_num]={record,m_directory,m_waveforms[slot_num],key};
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
			auto scancode=m_channel_to_scancode[ch];
			auto key=scancode==0?nullptr:m_keyboard.keyFromScancode(scancode);

			m_channel_data[ch]={record,m_channels[ch],key};
			}
		}
	m_state_flags=0;
	}

void Session::keyHighlight(uint8_t scancode)
	{
	auto key_active=m_keyboard.keyFromScancode(scancode);
	if(key_active!=nullptr)
		{
		if(r_key_active!=nullptr)
			{r_key_active->colorBorderSet({0.5f,0.5f,0.5f,1.0f});}
		key_active->colorBorderSet(COLORS[ColorID::GREEN]);
		r_key_active=key_active;
		}
	}

void Session::audioServerConnect()
	{
	audioServerDisconnect();
	m_engine.multioutputSet(flagGet(MULTIOUTPUT));
	m_connection=AudioConnection::create(m_title.begin(),m_engine);

//	FIXME Instead of relying on a delay, use a callback. This event must be
//	issued from the process function to ensure that the audio thread has started
//	ugh.
	sleep(1);

//	Post channel volume messages
	auto channel=m_channels.begin();
	auto channels_end=m_channels.end();
	auto k=0;
	while(channel!=channels_end)
		{
		auto value=channel->gainGet();
		m_engine.eventPost(k|MIDIConstants::StatusCodes::CONTROLLER
			,MIDIConstants::ControlCodes::CHANNEL_VOLUME
			,dBToAmplitude(value));
		++channel;
		++k;
		}

	m_state_flags&=~RESTART_NEEDED;
	}

void Session::audioServerDisconnect()
	{
	if(m_connection!=nullptr)
		{m_connection->destroy();}
	m_connection=nullptr;
	m_state_flags&=~RESTART_NEEDED;
	}

void Session::save()
	{
//	TODO
	}

void Session::save(const char* filename)
	{
	char buffer[32];
	auto writer=SessionFileWriter::create(filename);
	SessionFileRecordImpl record_out;

	record_out.sectionLevelSet(0);
	record_out.sectionTitleSet(m_title);
	sprintf(buffer,"%u",m_slot_active + 1);
	record_out.propertySet("Active slot",buffer);
	record_out.propertySet("Description",descriptionGet().begin());

	sprintf(buffer,"%.7g",masterGainGet());
	record_out.propertySet("Master gain/dB",buffer);
	record_out.propertySet("Options",stringFromOptions(flagsGet(),FLAG_NAMES));

//	TODO Save other data not interpreted by Anja
	writer->recordWrite(record_out);
	record_out.clear();

	//	Loop through all waveform slots
		{
		auto waveform=m_waveform_data.begin();
		auto waveforms_end=m_waveform_data.end();
		auto k=0;
		while(waveform!=waveforms_end)
			{
			record_out.sectionLevelSet(1);
			sprintf(buffer,"Slot %u",k+1);
			record_out.sectionTitleSet(buffer);
			waveform->dataGet(record_out);
			writer->recordWrite(record_out);
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
			record_out.sectionLevelSet(1);
			sprintf(buffer,"Channel %u",k+1);
			record_out.sectionTitleSet(buffer);
			channel->dataGet(record_out);
			writer->recordWrite(record_out);
			++k;
			++channel;
			}
		}
	m_state_flags&=~SESSION_DIRTY;
	}

float Session::masterGainGet() const noexcept
	{
	return amplitudeToDb(m_engine.masterGainGet());
	}

Session& Session::masterGainSet(float value) noexcept
	{
	m_engine.masterGainSet(dBToAmplitude(value));
	m_state_flags|=SESSION_DIRTY;
	return *this;
	}
