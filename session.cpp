#ifdef __WAND__
target[name[session.o] type[object]]
#endif

#include "session.h"
#include "waveformrange.h"
#include "sessionfilereader.h"
#include "sessionfilerecordimpl.h"
#include "audioconnection.h"
#include "framework/localeguard.h"
#include "framework/pathutils.h"

#include <cstring>

void Session::waveformsClear()
	{
	//	Reset waveforms
		{
		auto ptr=m_waveforms.begin();
		auto ptr_end=m_waveforms.end();
		while(ptr!=ptr_end)
			{
			ptr->clear();
			ptr->append(WaveformRange::s_null.beginFull(),WaveformRange::s_null.lengthFull());
			ptr->sampleRateSet(1000.0);
			++ptr;
			}
		}

	//	Reset waveform data
		{
		auto ptr=m_waveform_data.begin();
		auto ptr_end=m_waveform_data.end();
		auto ptr_waveform=m_waveforms.begin();
		while(ptr!=ptr_end)
			{
			ptr->waveformSet(*ptr_waveform);
			++ptr;
			++ptr_waveform;
			}

		}

	//	Reset scancodes
		{
		memset(m_scancode_to_slot.begin(),-1,sizeof(m_scancode_to_slot));
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
				++k;
				}
			scancode_prev=val;
			++ptr;
			}
		}

	//	Reset MIDI keys
		{
		memset(m_midikey_to_slot.begin(),-1,sizeof(m_midikey_to_slot));
		for(uint8_t k=36;k<128;++k)
			{
			m_midikey_to_slot[k]=k-36;
			}
		}
	}

void Session::load(const char* filename)
	{
	auto reader=SessionFileReader::create(filename);
	SessionFileRecordImpl record;

	if(!reader->recordNextGet(record))
		{throw "Invalid session file";}

	if(record.sectionLevelGet()!=0)
		{throw "Invalid session file";}

	waveformsClear();

	m_filename=filename;
	m_directory=parentDirectory(realpath(m_filename));
	titleSet(record.titleGet());

	while(reader->recordNextGet(record))
		{
		if(record.sectionLevelGet()==0)
			{break;}

		auto title_ptr=record.titleGet().begin();
		if(strncmp(title_ptr,"Slot ",5)==0)
			{
			title_ptr+=5;
			int slot_num;
				{
				LocaleGuard locale("C");
				slot_num=atol(title_ptr);
				}
			if(slot_num<1 || slot_num>128)
				{throw "Invalid slot number";}
			slot_num-=1;

			waveformDataSet({record,m_directory,m_waveforms[slot_num]},slot_num);
			}
		}
	}

void Session::audioServerConnect()
	{
	audioServerDisconnect();
	m_connection=AudioConnection::create(m_title.begin());
	}

void Session::audioServerDisconnect()
	{
	if(m_connection!=nullptr)
		{m_connection->destroy();}
	m_connection=nullptr;
	}
