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
#include "../sessiondata/channelview.hpp"

#include <memory>

namespace Anja
	{
	class ChannelStrip
		{
		public:
			explicit ChannelStrip(Container& cnt,const ChannelView& channel);

			enum class TextEntryId:int{LABEL,FADETIME,GAIN};
			enum class ColorViewId:int{COLOR};
			enum class SliderId:int{GAIN};
			enum class PopupId:int{COLOR_SELECT};

			void changed(TextEntry& entry,TextEntryId id);
			void clicked(ColorView& entry,ColorViewId id);
			void changed(Slider& slider,SliderId id);
			void dismiss(Dialog<ColorPicker>& dlg,PopupId id);
			void confirmPositive(Dialog<ColorPicker>& dlg,PopupId id);


		private:
			ChannelView m_channel;
		//	const ArraySimple<ColorRGBA>& r_color_pal;
			Box m_box;
				TextEntry m_name;
				ColorView m_color;
				Label m_ft_label;
			//	Knob m_ft_knob;
				TextEntry m_ft_input;
				Slider m_gain_slider;
				TextEntry m_gain_input;
			std::unique_ptr<Dialog<ColorPicker>> m_color_dlg;
		};
	}

#endif
