//	{
//	"targets":[{"name":"session.hpp","type":"include"}]
//	,"dependencies_extra":[{"ref":"session.o","rel":"implementation"}]
//	}

#ifndef ANJA_SESSION_HPP
#define ANJA_SESSION_HPP

namespace Anja
	{
	class Session
		{
		public:
			Session& operator=(const Session&)=delete;
			Session(const Session&)=delete;

			Session():m_engine(m_waveforms),m_connection(nullptr)
				,r_key_active(nullptr),m_slot_active(0),m_state_flags(0)
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
				{
				m_title=title_new;
				m_state_flags|=RESTART_NEEDED|SESSION_DIRTY;
				}

			const ArrayDynamicShort<char>& titleGet() const noexcept
				{return m_title;}

			void descriptionSet(const ArrayDynamicShort<char>& description_new) noexcept
				{
				m_description=description_new;
				m_state_flags|=SESSION_DIRTY;
				}

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

			unsigned int flagGet(unsigned int index) noexcept
				{return m_flags&index;}


			Session& flagsSet(unsigned int flags) noexcept
				{
				if( (m_flags&MULTIOUTPUT) != (flags&MULTIOUTPUT) )
					{m_state_flags|=RESTART_NEEDED;}
				m_flags|=flags;
				m_state_flags|=SESSION_DIRTY;
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
				{return flagsUnset(1<<index);}



			ColorRGBA* colorPresetsBegin() noexcept
				{return m_color_presets.begin();}

			size_t colorPresetsCountGet() const noexcept
				{return m_color_presets.length();}


			float masterGainGet() const noexcept;

			Session& masterGainSet(float value) noexcept;

			bool restartNeeded() const noexcept
				{return m_state_flags&RESTART_NEEDED;}

			bool dirtyIs() const noexcept;

			void dirtyClear() noexcept
				{m_state_flags&=~SESSION_DIRTY;}

		private:
			AudioEngineAnja m_engine;
			AudioConnection* m_connection;

			ArrayFixed<uint8_t,Wavetable::length()> m_slot_to_scancode;;
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
			ArrayFixed<ColorRGBA,64> m_color_presets;


			KeyboardLayout::KeyDescriptor* r_key_active;
			uint8_t m_slot_active;
			static constexpr unsigned int MULTIOUTPUT=0x1;
			unsigned int m_flags;


			static constexpr unsigned int RESTART_NEEDED=0x1;
			static constexpr unsigned int SESSION_DIRTY=0x2;

			unsigned int m_state_flags;
		};
	}
#endif
