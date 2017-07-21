//@	{
//@	"targets":[{"name":"session.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"session.o","rel":"implementation"}]
//@	}

#ifndef ANJA_SESSION_HPP
#define ANJA_SESSION_HPP

#include "../common/arraydynamicshort.hpp"
#include "../common/arraysimple.hpp"
#include "waveformproxy.hpp"
#include "channelproxy.hpp"
#include "wavetable.hpp"
#include "channelmixer.hpp"

namespace Anja
	{
	class Session
		{
		public:
			Session():m_slot_active(0),m_state_flags(0)
				{clear();}

			template<class ProgressCallback>
			explicit Session(const char* filename,ProgressCallback& cb):
				Session(filename,[](void* obj,WaveformProxy& self,float status)
					{
					auto cb=reinterpret_cast<ProgressCallback*>(obj);
					cb->progressLoad(self,status);
					},&cb)
				{}

			template<class ProgressCallback>
			void load(const char* filename,ProgressCallback& cb)
				{*this=Session(filename,cb);}

			void save(const char* filename);

			const String& filenameGet() const noexcept
				{return m_filename;}

			const String& directoryGet() const noexcept
				{return m_directory;}



			void titleSet(String&& title_new) noexcept
				{
				if(title_new!=m_title)
					{
					m_title=std::move(title_new);
					m_state_flags|=SESSION_DIRTY;
					}
				}

			void titleSet(const char* title_new) noexcept
				{
				if(m_title!=title_new)
					{
					m_title=String(title_new);
					m_state_flags|=SESSION_DIRTY;
					}
				}

			const String& titleGet() const noexcept
				{return m_title;}

			void descriptionSet(String&& description_new) noexcept
				{
				if(m_description!=description_new)
					{
					m_description=std::move(description_new);
					m_state_flags|=SESSION_DIRTY;
					}
				}

			void descriptionSet(const char* description_new) noexcept
				{
				if(m_description!=description_new)
					{
					m_description=String(description_new);
					m_state_flags|=SESSION_DIRTY;
					}
				}

			const String& descriptionGet() const noexcept
				{return m_description;}


			static constexpr unsigned int MULTIOUTPUT=0x1;
			static constexpr unsigned int ALLOW_CHANNEL_OVERRIDE=0x2;

			static const char* const* flagNames() noexcept;

			unsigned int flagsGet() const noexcept
				{return m_flags;}

			unsigned int flagGet(unsigned int index) noexcept
				{return m_flags&index;}


			Session& flagsSet(unsigned int flags) noexcept
				{
				if((m_flags|flags)!=m_flags)
					{m_state_flags|=SESSION_DIRTY;}
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
				{return flagsUnset(1<<index);}




			void waveformsClear();

			Waveform& waveformGet(uint8_t slot) noexcept
				{return m_waveforms[slot];}

			const Waveform& waveformGet(uint8_t slot) const noexcept
				{return m_waveforms[slot];}

			WaveformProxy waveformViewGet(uint8_t slot) noexcept
				{return WaveformProxy(m_waveforms[slot],m_waveform_data[slot],m_directory,m_fs,slot);}

			WaveformData& waveformData(uint8_t slot) noexcept
				{return m_waveform_data[slot];}

			const WaveformData& waveformData(uint8_t slot) const noexcept
				{return m_waveform_data[slot];}

			ChannelProxy channelViewGet(int ch) noexcept
				{return ChannelProxy(m_channels[ch],m_channel_data[ch]);}

			ArraySimple<String> channelLabelsGet() const;

			const String& channelLabelGet(int ch) const noexcept
				{
				assert(ch>=0 && ch<16);
				return m_channel_data[ch].label();
				}

			ArraySimple<ColorRGBA> channelColorsGet() const;

			int channelsCountGet() noexcept
				{return m_channel_data.length();}

			const Channel& channel(unsigned int index) const noexcept
				{return m_channels[index];}

			Channel& channel(unsigned int index) noexcept
				{return m_channels[index];}







			uint8_t slotActiveGet() const noexcept
				{return m_slot_active;}

			void slotActiveSet(uint8_t slot)
				{m_slot_active=slot;}

			int slotsCountGet() const noexcept
				{return Wavetable::length();}


			void channelsClear();

			void clear();





			const auto& colorPresetsGet() const noexcept
				{return m_color_presets;}



			float gainGet() const noexcept
				{return m_gain;}

			void gainSet(float value) noexcept
				{
				m_state_flags|=(std::abs(value - m_gain)>1e-4? SESSION_DIRTY : 0);
				m_gain=value;
				}

			bool dirtyIs() const noexcept;

			void dirtyClear() noexcept
				{m_state_flags&=~SESSION_DIRTY;}

			Session& colorPresetsSet(const ColorRGBA* begin,const ColorRGBA* end);

			bool loadPossible(const char* filename) const;

			template<class ProgressCallback>
			Session& sampleRate(double fs,ProgressCallback& cb_obj)
				{
				auto cb=[](void* obj,WaveformProxy& self,float status)
					{
					auto cb=reinterpret_cast<ProgressCallback*>(obj);
					cb->progressResample(self,status);
					};
				return sampleRate(fs,cb,&cb_obj);
				}

			Session& engineStopped() noexcept
				{
				m_fs=0;
				return *this;
				}

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

			unsigned int m_flags;
			double m_fs;

			static constexpr unsigned int SESSION_DIRTY=0x2;

			unsigned int m_state_flags;

			void keyHighlight(uint8_t scancode);
			void keyReset(uint8_t scancode);

			typedef void (*progress_callback)(void* obj,WaveformProxy& self,float status);
			explicit Session(const char* filename,progress_callback cb,void* obj);
			Session& sampleRate(double fs,progress_callback,void* obj);
		};
	}
#endif
