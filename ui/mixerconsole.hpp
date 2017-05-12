//@	{
//@  "targets":[{"name":"mixerconsole.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"mixerconsole.o","rel":"implementation"}]
//@	}

#ifndef ANJA_MIXERCONSOLE_HPP
#define ANJA_MIXERCONSOLE_HPP

#include "box.hpp"
#include "scrolledwindow.hpp"
#include "../common/arraysimple.hpp"
#include "channelstrip.hpp"
#include "../common/arraydynamicshort.hpp"

namespace Anja
	{
	class Session;

	class MixerConsole
		{
		public:
			MixerConsole(MixerConsole&&)=delete;
			MixerConsole& operator=(MixerConsole&&)=delete;

			explicit MixerConsole(Container& cnt,Session& session);

			template<class ColorRange>
			MixerConsole& colorPresets(const ColorRange& colors)
				{return colorPresets(colors.begin(),colors.end());}

			MixerConsole& colorPresets(const ColorRGBA* begin,const ColorRGBA* end);

			enum class SliderId:int{MASTER_GAIN};

			void changed(Slider& slider,SliderId id);

			enum class TextEntryId:int{MASTER_GAIN};

			void changed(TextEntry& entry,TextEntryId id);

			template<class Callback>
			MixerConsole& channelsCallback(Callback& cb_obj) noexcept
				{
				auto k=0;
				std::for_each(m_strips.begin(),m_strips.end(),[&cb_obj,&k](ChannelStrip& strip)
					{
					strip.callback(cb_obj,k);
					++k;
					});
				return *this;
				}

			template<class Callback,class IdType>
			MixerConsole& callback(Callback& cb_obj,IdType id)
				{
				auto cb=[](void* cb_obj,MixerConsole& self,int id)
					{
					reinterpret_cast<Callback*>(cb_obj)->masterGainChanged(self,static_cast<IdType>(id));
					};
				m_id=static_cast<int>(id);
				r_cb_obj=&cb_obj;
				m_cb=cb;
				return *this;
				}

			float masterGain() const noexcept
				{return m_master_gain;}

		private:
			int m_id;
			void* r_cb_obj;
			void (*m_cb)(void* cb_obj,MixerConsole& self,int id);

			float m_master_gain;
			Box m_sections;
				ScrolledWindow m_channels;
					Box m_strip_box;
						ArrayDynamicShort<Separator> m_separators;
						ArraySimple<ChannelStrip> m_strips;
				Box m_master;
					Label m_master_label;
					Slider m_master_slider;
					TextEntry m_master_entry;
		};
	}

#endif
