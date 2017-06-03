//@	{
//@	 "targets":[{"name":"channelstrip.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"channelstrip.o","rel":"implementation"}]
//@	}

#ifndef ANJA_CHANNELSTRIP_HPP
#define ANJA_CHANNELSTREP_HPP

#include "box.hpp"
#include "textentry.hpp"
#include "colorview.hpp"
#include "label.hpp"
#include "slider.hpp"
#include "colorpicker.hpp"
#include "dialog.hpp"
#include "separator.hpp"
#include "knob.hpp"
#include "../sessiondata/channelproxy.hpp"

#include <memory>

namespace Anja
	{
	class ChannelStrip
		{
		public:
			explicit ChannelStrip(Container& cnt,const ChannelProxy& channel);

			enum class TextEntryId:int{LABEL,FADETIME,GAIN};
			enum class ColorViewId:int{COLOR};
			enum class SliderId:int{GAIN};
			enum class KnobId:int{FADETIME};
			enum class PopupId:int{COLOR_SELECT};

			ChannelStrip& operator=(ChannelStrip&&)=delete;
			ChannelStrip(ChannelStrip&&)=delete;

			ChannelStrip& colorPresets(const ColorRGBA* colors_begin,const ColorRGBA* colors_end);

			const String& name() const noexcept
				{return m_channel.labelGet();}

			const ColorRGBA& color() const noexcept
				{return m_channel.colorGet();}

			template<class Callback,class IdType>
			ChannelStrip& callback(Callback& cb,IdType id) noexcept
				{
				m_id=id;
				r_cb_obj=&cb;
				m_vtable=Vtable(cb,id);
				return *this;
				}

			void changed(TextEntry& entry,TextEntryId id);
			void clicked(ColorView& entry,ColorViewId id);
			void changed(Slider& slider,SliderId id);
			void dismiss(Dialog<ColorPicker>& dlg,PopupId id);
			void confirmPositive(Dialog<ColorPicker>& dlg,PopupId id);
			void changed(Knob& knob,KnobId id);

			ChannelStrip& focus()
				{
				m_name.focus();
				return *this;
				}

			ChannelStrip& channel(const ChannelProxy& ch);


		private:
			struct Vtable
				{
				Vtable():name_changed(nullptr),color_changed(nullptr),color_presets_changed(nullptr)
					{}

				template<class Callback,class IdType>
				Vtable(Callback& cb_obj,IdType id)
					{
					name_changed=[](void* cb_obj,ChannelStrip& self,int id)
						{reinterpret_cast<Callback*>(cb_obj)->nameChanged(self,static_cast<IdType>(id));};
					color_changed=[](void* cb_obj,ChannelStrip& self,int id)
						{reinterpret_cast<Callback*>(cb_obj)->colorChanged(self,static_cast<IdType>(id));};
					color_presets_changed=[](void* cb_obj,ColorPicker& self)
						{reinterpret_cast<Callback*>(cb_obj)->colorPresetsChanged(self);};
					}

				void (*name_changed)(void* cb_obj,ChannelStrip& self,int id);
				void (*color_changed)(void* cb_obj,ChannelStrip& self,int id);
				void (*color_presets_changed)(void* cb_obj,ColorPicker& self);
				};

			int m_id;
			void* r_cb_obj;
			Vtable m_vtable;

			ChannelProxy m_channel;
			const ColorRGBA* r_color_presets_begin;
			const ColorRGBA* r_color_presets_end;
			Box m_box;
				TextEntry m_name;
				ColorView m_color;
				Separator m_sep_a;
				Label m_ft_label;
				Knob m_ft_knob;
				TextEntry m_ft_input;
				Separator m_sep_b;
				Slider m_gain_slider;
				TextEntry m_gain_input;
			std::unique_ptr<Dialog<ColorPicker>> m_color_dlg;
		};
	}

#endif
