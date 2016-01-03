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

class AudioConnection;

class Session
	{
	public:
		Session& operator=(const Session&)=delete;
		Session(const Session&)=delete;

		Session():m_connection(nullptr)
			{
			waveformsClear();
			}

		Session(const char* filename):m_connection(nullptr)
			{
			load(filename);
			audioServerConnect();
			}

		~Session()
			{
			audioServerDisconnect();
			}

		void load(const char* filename);

		const ArrayDynamicShort<char>& filenameGet() const
			{return m_filename;}

		const ArrayDynamicShort<char>& directoryGet() const
			{return m_directory;}

		void waveformsClear();

		const WaveformData& waveformDataGet(uint8_t slot) const
			{return m_waveform_data[slot];}

		WaveformData& waveformDataGet(uint8_t slot)
			{return m_waveform_data[slot];}

		const Waveform& waveformGet(uint8_t slot)
			{return m_waveforms[slot];}



		void titleSet(const ArrayDynamicShort<char>& title_new)
			{m_title=title_new;}

		const ArrayDynamicShort<char>& titleGet() const
			{return m_title;}



		KeyboardLayout& keyboardLayoutGet()
			{return m_keyboard;}

		const KeyboardLayout& keyboardLayoutGet() const
			{return m_keyboard;}

		uint8_t scancodeToSlot(uint8_t scancode) const
			{return m_scancode_to_slot[scancode];}

		uint8_t slotActiveGet() const
			{return m_slot_active;}

		void slotActiveSet(uint8_t slot);




		WaveformData& waveformDataFromMIDI(uint8_t midikey)
			{return m_waveform_data[m_midikey_to_slot[midikey]];}

		const WaveformData& waveformDataFromMIDI(uint8_t midikey) const
			{return m_waveform_data[m_midikey_to_slot[midikey]];}



		void audioServerConnect();

		void audioServerDisconnect();

		AudioConnection* audioConnectionGet()
			{return m_connection;}

	private:
		AudioConnection* m_connection;

		ArrayFixed<uint8_t,128> m_slot_to_scancode;
		ArrayFixed<uint8_t,128> m_midikey_to_slot;
		ArrayFixed<uint8_t,128> m_scancode_to_slot;
		ArrayFixed<Waveform,128> m_waveforms;
		ArrayFixed<WaveformData,128> m_waveform_data;
		KeyboardLayout m_keyboard;

		ArrayDynamicShort<char> m_filename;
		ArrayDynamicShort<char> m_directory;
		ArrayDynamicShort<char> m_title;
		uint8_t m_slot_active;

		static constexpr uint32_t ENGINE_ONLINE=1;
		static constexpr uint32_t MODE_RECORD=2;
		static constexpr uint32_t VIEW_LIGHT=4;
	};

#endif
