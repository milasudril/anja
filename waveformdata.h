#ifdef __WAND__
target[name[waveformdata.h] type[include]]
dependency[waveformdata.o]
#endif

#ifndef WAVEFORMDATA_H
#define WAVEFORMDATA_H

#include "framework/color.h"
#include "waveformrange.h"
#include "waveformstorage.h"
#include <string>

class SessionFileRecord;

class WaveformData
	{
	public:
		WaveformData():m_waveform_range(nullptr,0,0)
			{}

		WaveformData(const SessionFileRecord& record
			,const ArrayDynamicShort<char>& load_path);

		void waveformRangeUpdate()
			{
			r_waveform->offsetBeginSet(m_waveform_range.offsetBeginGet());
			r_waveform->offsetEndSet(m_waveform_range.offsetEndGet());
			}

		WaveformRange& waveformRangeGet()
			{return m_waveform_range;}

		void waveformSet(WaveformStorage& waveform_new)
			{
			r_waveform=&waveform_new;
			m_waveform_range=*r_waveform;
			}

		const ArrayDynamicShort<char>& filenameGet() const
			{return m_filename;}

		void fileLoad(const ArrayDynamicShort<char>& filename
			,const ArrayDynamicShort<char>& load_path);

		const ArrayDynamicShort<char>& descriptionGet() const
			{return m_description;}

		void descriptionSet(const ArrayDynamicShort<char>& description);

		const ArrayDynamicShort<char>& keyLabelGet() const
			{return m_key_label;}

		const ColorRGBA& keyColorGet() const
			{return m_color;}

		ColorRGBA& keyColorGet()
			{return m_color;}

	private:
		ArrayDynamicShort<char> m_filename;
		ArrayDynamicShort<char> m_description;
		ArrayDynamicShort<char> m_key_label;
		ColorRGBA m_color;
		WaveformRange m_waveform_range;
		WaveformStorage* r_waveform;
		uint8_t m_scancode;
		uint8_t m_midi_note;
	};

#endif
