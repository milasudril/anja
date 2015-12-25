#ifdef __WAND__
target[name[session.o] type[object]]
#endif

#include "session.h"
#include "waveformrange.h"

void Session::waveformsClear()
	{
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
	}
