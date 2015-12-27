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

#include <map>

class Session
	{
	public:
		Session():m_keys(compareLexicographical< ArrayDynamicShort<char> >)
			{waveformsClear();}

		void waveformsClear();

		void waveformDataSave(const WaveformData& data,uint8_t slot);

		WaveformData& waveformDataGet(uint8_t slot)
			{return m_waveform_data[slot];}

		const WaveformData& waveformDataGet(uint8_t slot) const
			{return m_waveform_data[slot];}

		WaveformData& waveformDataFromScancode(uint8_t scancode)
			{return m_waveform_data[m_scancode_to_slot[scancode]];}

		const WaveformData& waveformDataFromScancode(uint8_t scancode) const
			{return m_waveform_data[m_scancode_to_slot[scancode]];}

		WaveformData& waveformDataFromMIDI(uint8_t midikey)
			{return m_waveform_data[m_midikey_to_slot[midikey]];}

		const WaveformData& waveformDataFromMIDI(uint8_t midikey) const
			{return m_waveform_data[m_midikey_to_slot[midikey]];}

		void titleSet(const ArrayDynamicShort<char>& title)
			{
			m_title.clear();
			m_title.append(title.begin(),title.length());
			}

		const char* titleGet() const
			{return m_title.begin();}

		void keySet(const ArrayDynamicShort<char>& key
			, const ArrayDynamicShort<char>& value)
			{m_keys[key]=value;}

	private:
		ArrayFixed<uint8_t,128> m_scancode_to_slot;
		ArrayFixed<uint8_t,128> m_midikey_to_slot;
		ArrayFixed<WaveformStorage,128> m_waveforms;
		ArrayFixed<WaveformData,128> m_waveform_data;
		KeyboardLayout m_keyboard;

		ArrayDynamicShort<char> m_title;
		std::map< ArrayDynamicShort<char>, ArrayDynamicShort<char>
			,decltype(&compareLexicographical< ArrayDynamicShort<char> >) > m_keys;

		float m_gain;

		static constexpr uint32_t ENGINE_ONLINE=1;
		static constexpr uint32_t MODE_RECORD=2;
		static constexpr uint32_t VIEW_LIGHT=4;
	};

#endif
