#ifdef __WAND__
target[name[waveformdata.h] type[include]]
#endif

#ifndef WAVEFORMDATA_H
#define WAVEFORMDATA_H

#include "framework/color.h"
#include "waveformrange.h"
#include <string>


class WaveformRange;

class WaveformData
	{
	public:
		WaveformData():m_waveform_range(WaveformRange::s_null)
			{}

		void waveformRangeSet(const WaveformRange& range)
			{m_waveform_range=range;}

		void waveformRangeSet(WaveformRange&& range)
			{m_waveform_range=std::move(range);}

	private:
		std::string m_filename;
		std::string m_description;
		ColorRGBA m_color;
		WaveformRange m_waveform_range;
		uint8_t m_scancode;
		uint8_t m_midi_note;
	};

#endif
