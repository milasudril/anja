//@	{
//@	"targets":[{"name":"session.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"session.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSION_HPP
#define ANJA_SESSION_HPP

#include "../common/arraydynamicshort.hpp"
#include "../common/arraysimple.hpp"
#include "waveformview.hpp"
#include "channelview.hpp"
#include "wavetable.hpp"
#include "channelmixer.hpp"

namespace Anja
	{
	class Session
		{
		public:
			Session():m_slot_active(0),m_state_flags(0)
				{clear();}

			explicit Session(const char* filename);

			void load(const char* filename)
				{*this=Session(filename);}

			void save(const char* filename);

			const String& filenameGet() const noexcept
				{return m_filename;}

			const String& directoryGet() const noexcept
				{return m_directory;}

			void waveformsClear();

			Waveform& waveformGet(uint8_t slot) noexcept
				{return m_waveforms[slot];}

			const Waveform& waveformGet(uint8_t slot) const noexcept
				{return m_waveforms[slot];}

			WaveformView waveformViewGet(uint8_t slot) noexcept
				{return WaveformView(m_waveforms[slot],m_waveform_data[slot],m_directory,slot);}

			ChannelView channelViewGet(int ch) noexcept
				{return ChannelView(m_channels[ch],m_channel_data[ch]);}

			ArraySimple<String> channelLabelsGet() const;

			ArraySimple<ColorRGBA> channelColorsGet() const;

			int channelsCountGet() noexcept
				{return m_channel_data.length();}

			const Channel& channelGet(unsigned int index) const noexcept
				{return m_channels[index];}

			Channel& channelGet(unsigned int index) noexcept
				{return m_channels[index];}



			void titleSet(const String& title_new) noexcept
				{
				m_title=title_new;
				m_state_flags|=RESTART_NEEDED|SESSION_DIRTY;
				}

			const String& titleGet() const noexcept
				{return m_title;}

			void descriptionSet(const String& description_new) noexcept
				{
				m_description=description_new;
				m_state_flags|=SESSION_DIRTY;
				}

			const String& descriptionGet() const noexcept
				{return m_description;}


			uint8_t slotActiveGet() const noexcept
				{return m_slot_active;}

			void slotActiveSet(uint8_t slot)
				{m_slot_active=slot;}

			int slotsCountGet() const noexcept
				{return Wavetable::length();}



			void channelsClear();

			void clear();

			static const char* const* flagNames() noexcept;

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



			const auto& colorPresetsGet() const noexcept
				{return m_color_presets;}



			float gainGet() const noexcept
				{return m_gain;}

			void gainSet(float value) noexcept
				{
				m_gain=value;
				m_state_flags|=SESSION_DIRTY;
				}


			bool restartNeeded() const noexcept
				{return m_state_flags&RESTART_NEEDED;}

			bool dirtyIs() const noexcept;

			void dirtyClear() noexcept
				{m_state_flags&=~SESSION_DIRTY;}

			Session& colorPresetsSet(const ColorRGBA* begin,const ColorRGBA* end);

		private:
			Wavetable m_waveforms;
			ChannelMixer m_channels;
			float m_gain;

			ArrayFixed<WaveformData,Wavetable::length()> m_waveform_data;
			ArrayFixed<ChannelData,ChannelMixer::length()> m_channel_data;
			int8_t m_slot_active;

			String m_filename;
			String m_directory;
			String m_title;
			String m_description;
			ArrayDynamicShort<ColorRGBA> m_color_presets;

			static constexpr unsigned int MULTIOUTPUT=0x1;
			unsigned int m_flags;

			static constexpr unsigned int RESTART_NEEDED=0x1;
			static constexpr unsigned int SESSION_DIRTY=0x2;

			unsigned int m_state_flags;

			void keyHighlight(uint8_t scancode);
			void keyReset(uint8_t scancode);
		};
	}
#endif
