#ifdef __WAND__
target[name[session.h] type[include]]
dependency[session.o]
#endif

#ifndef SESSION_H
#define SESSION_H

#include "wavetable.h"
#include "channelmixer.h"
#include "waveformdata.h"
#include "channeldata.h"
#include "audioengineanja.h"
#include "framework/arraydynamicshort.h"
#include "framework/keyboardlayout.h"
#include "framework/range.h"

class AudioConnection;

class Session
	{
	public:
		Session& operator=(const Session&)=delete;
		Session(const Session&)=delete;

		Session():m_engine(m_waveforms),m_connection(nullptr)
			,r_key_active(nullptr),m_slot_active(0)
			{clear();}

		Session(const char* filename):m_engine(m_waveforms)
			,m_connection(nullptr),r_key_active(nullptr),m_slot_active(0)
			{
			load(filename);
			}

		~Session()
			{
			audioServerDisconnect();
			}

		void load(const char* filename);

		void save();

		void save(const char* filename);

		const ArrayDynamicShort<char>& filenameGet() const noexcept
			{return m_filename;}

		const ArrayDynamicShort<char>& directoryGet() const noexcept
			{return m_directory;}



		void waveformsClear();

		const WaveformData& waveformDataGet(uint8_t slot) const noexcept
			{return m_waveform_data[slot];}

		WaveformData& waveformDataGet(uint8_t slot) noexcept
			{return m_waveform_data[slot];}

		const Waveform& waveformGet(uint8_t slot) noexcept
			{return m_waveforms[slot];}



		void titleSet(const ArrayDynamicShort<char>& title_new) noexcept
			{m_title=title_new;}

		const ArrayDynamicShort<char>& titleGet() const noexcept
			{return m_title;}

		void descriptionSet(const ArrayDynamicShort<char>& description_new) noexcept
			{m_description=description_new;}

		const ArrayDynamicShort<char>& descriptionGet() const noexcept
			{return m_description;}



		KeyboardLayout& keyboardLayoutGet() noexcept
			{return m_keyboard;}

		const KeyboardLayout& keyboardLayoutGet() const noexcept
			{return m_keyboard;}

		uint8_t scancodeToSlot(uint8_t scancode) const noexcept
			{return m_scancode_to_slot[scancode];}

		uint8_t slotActiveGet() const noexcept
			{return m_slot_active;}

		void slotActiveSet(uint8_t slot)
			{
			m_slot_active=slot;
			keyHighlight(m_slot_to_scancode[slot]);
			}



		void channelsClear();

		ChannelData* channelDataBegin() noexcept
			{return m_channel_data.begin();}

		unsigned int channelsCountGet() noexcept
			{return m_channel_data.length();}

		const ChannelData& channelDataGet(unsigned int index) const noexcept
			{return m_channel_data[index];}

		ChannelData& channelDataGet(unsigned int index)
			{return m_channel_data[index];}

		const Channel& channelGet(unsigned int index) const noexcept
			{return m_channels[index];}

		Channel& channelGet(unsigned int index) noexcept
			{return m_channels[index];}

		uint8_t scancodeToChannel(uint8_t scancode) const noexcept
			{return m_scancode_to_channel[scancode];}



		void clear();




		WaveformData& waveformDataFromMIDI(uint8_t midikey) noexcept
			{return m_waveform_data[m_midikey_to_slot[midikey]];}

		const WaveformData& waveformDataFromMIDI(uint8_t midikey) const noexcept
			{return m_waveform_data[m_midikey_to_slot[midikey]];}



		void audioServerConnect();

		void audioServerDisconnect();

		AudioConnection* audioConnectionGet() noexcept
			{return m_connection;}

		AudioEngineAnja& audioEngineGet() noexcept
			{return m_engine;}

		bool connectedIs() const noexcept
			{return m_connection!=nullptr;}



		void keyHighlight(uint8_t scancode);



		static const char* FLAG_NAMES[];

		unsigned int flagsGet() const noexcept
			{return m_flags;}

		Session& flagsSet(unsigned int flags) noexcept
			{
			m_flags|=flags;
			return *this;
			}

		Session& flagsUnset(unsigned int flags) noexcept
			{
			m_flags&=~flags;
			return *this;
			}

		Session& flagSet(unsigned int index) noexcept
			{return flagsSet(1<<index);}

		Session& flagUnset(unsigned int index) noexcept
			{return flagsSet(1<<index);}


	private:
		AudioEngineAnja m_engine;
		AudioConnection* m_connection;

		ArrayFixed<uint8_t,Wavetable::length()> m_slot_to_scancode;
		ArrayFixed<uint8_t,128> m_midikey_to_slot;
		ArrayFixed<uint8_t,128> m_scancode_to_slot;
		Wavetable m_waveforms;
		ChannelMixer m_channels;
		ArrayFixed<WaveformData,Wavetable::length()> m_waveform_data;
		ArrayFixed<ChannelData,ChannelMixer::length()> m_channel_data;
		KeyboardLayout m_keyboard;


		ArrayFixed<uint8_t,ChannelMixer::length()> m_channel_to_scancode;
		ArrayFixed<uint8_t,128> m_scancode_to_channel;

		ArrayDynamicShort<char> m_filename;
		ArrayDynamicShort<char> m_directory;
		ArrayDynamicShort<char> m_title;
		ArrayDynamicShort<char> m_description;
		ArrayDynamicShort<ColorRGBA> m_color_presets;

		KeyboardLayout::KeyDescriptor* r_key_active;
		uint8_t m_slot_active;
		static constexpr unsigned int MULTIOUTPUT=1;
		unsigned int m_flags;
	};

#endif
