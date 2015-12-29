#ifdef __WAND__
target[name[session.h] type[include]]
dependency[session.o]
#endif

#ifndef SESSION_H
#define SESSION_H

#include "framework/array_fixed.h"
#include "framework/arraydynamicshort.h"
#include "framework/keyboardlayout.h"
#include "framework/range.h"
#include "waveformdata.h"
#include "waveformstorage.h"

class Session
	{
	public:
		Session& operator=(const Session&)=delete;
		Session(const Session&)=delete;

		Session()
			{waveformsClear();}

		Session(const char* filename)
			{load(filename);}

		void load(const char* filename);

		const ArrayDynamicShort<char>& filenameGet() const
			{return m_filename;}

		const ArrayDynamicShort<char>& directoryGet() const
			{return m_directory;}



		void waveformsClear();

		void waveformDataSet(const WaveformData& data,uint8_t slot)
			{
			m_waveform_data[slot]=data;
			m_waveform_data[slot].waveformSet(m_waveforms[slot]);
			}

		const WaveformData& waveformDataGet(uint8_t slot) const
			{return m_waveform_data[slot];}



		void titleSet(const ArrayDynamicShort<char>& title_new)
			{m_title=title_new;}

		const ArrayDynamicShort<char>& titleGet() const
			{return m_title;}



		KeyboardLayout& keyboardLayoutGet()
			{return m_keyboard;}

		const KeyboardLayout& keyboardLayoutGet() const
			{return m_keyboard;}



		WaveformData& waveformDataFromScancode(uint8_t scancode)
			{return m_waveform_data[m_scancode_to_slot[scancode]];}

		const WaveformData& waveformDataFromScancode(uint8_t scancode) const
			{return m_waveform_data[m_scancode_to_slot[scancode]];}

		WaveformData& waveformDataFromMIDI(uint8_t midikey)
			{return m_waveform_data[m_midikey_to_slot[midikey]];}

		const WaveformData& waveformDataFromMIDI(uint8_t midikey) const
			{return m_waveform_data[m_midikey_to_slot[midikey]];}



	private:
		ArrayFixed<uint8_t,128> m_scancode_to_slot;
		ArrayFixed<uint8_t,128> m_midikey_to_slot;
		ArrayFixed<WaveformStorage,128> m_waveforms;
		ArrayFixed<WaveformData,128> m_waveform_data;
		KeyboardLayout m_keyboard;

		ArrayDynamicShort<char> m_filename;
		ArrayDynamicShort<char> m_directory;
		ArrayDynamicShort<char> m_title;

		static constexpr uint32_t ENGINE_ONLINE=1;
		static constexpr uint32_t MODE_RECORD=2;
		static constexpr uint32_t VIEW_LIGHT=4;
	};

#endif
