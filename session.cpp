#ifdef __WAND__
target[name[session.o] type[object]]
#endif

#include "session.h"
#include "waveformrange.h"

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
			ptr->waveformRangeSet(*ptr_waveform);
			++ptr;
			++ptr_waveform;
			}

		}

	//	Reset scancodes
		{
		auto ptr=m_keyboard.typingAreaScancodesBegin();
		auto ptr_end=m_keyboard.typingAreaScancodesEnd();
		uint8_t scancode_prev=0;
		uint8_t k=0;
		while(ptr!=ptr_end)
			{
			auto val=*ptr;
			if(val!=scancode_prev && val!=0)
				{
				m_scancode_to_slot[k]=val;
				++k;
				}
			scancode_prev=val;
			++ptr;
			}
		}

	//	Reset MIDI keys
		{
		for(uint8_t k=36;k<128;++k)
			{
			m_midikey_to_slot[36]=k-36;
			}
		}
	}
