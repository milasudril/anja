#ifdef __WAND__
target[name[session.h] type[include]]
dependency[session.o]
#endif

#ifndef SESSION_H
#define SESSION_H

#include "framework/array_fixed.h"
#include "framework/keyboardlayout.h"
#include "waveformdata.h"
#include "waveformstorage.h"

class Session
	{
	public:
		Session()
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


	private:
		ArrayFixed<uint8_t,128> m_scancode_to_slot;
		ArrayFixed<uint8_t,128> m_midikey_to_slot;
		ArrayFixed<WaveformStorage,128> m_waveforms;
		ArrayFixed<WaveformData,128> m_waveform_data;
		KeyboardLayout m_keyboard;
		float m_gain;

		static constexpr uint32_t ENGINE_ONLINE=1;
		static constexpr uint32_t MODE_RECORD=2;
		static constexpr uint32_t VIEW_LIGHT=4;
	};

#endif
