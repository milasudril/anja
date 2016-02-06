#ifdef __WAND__
target[name[channelstriphandler.h] type[include]]
dependency[channelstriphandler.o]
#endif

#ifndef CHANNELSTRIPHANDLER_H
#define CHANNELSTRIPHANDLER_H

#include "framework/textbox.h"
#include "framework/colorview.h"
#include "framework/valueinput.h"
#include "framework/colorpicker.h"
#include "framework/color.h"
#include "framework/array_simple.h"

class Window;

class ChannelStripHandler:public Textbox::EventHandler
	,public ColorView::EventHandler,public ValueInput::EventHandler
	,public ColorPicker::EventHandler
	{
	public:
		class EventHandler
			{
			public:
				virtual void onLabelChange(unsigned int channel,const char* label)=0;
				virtual void onFadeTimeChange(unsigned int channel,float time)=0;
				virtual void onGainChange(unsigned int channel,float value)=0;
				virtual void onColorChange(unsigned int channel,const ColorRGBA& value)=0;
			};

		ChannelStripHandler(EventHandler& handler);
		~ChannelStripHandler();

		void textGet(ValueInput& source,double value,TextBuffer& buffer);
		double valueGet(ValueInput& source,const char* text);
		double valueMap(ValueInput& source,double x) const noexcept;
		double valueMapInverse(ValueInput& source,double y) const noexcept;

		void onLeave(Textbox& source);

		void onActionPerform(ColorView& source);

		void onConfirmed(ColorPicker::Tag x);

	private:
		EventHandler* r_handler;

		Window* m_colordlg;
		ColorPicker* m_picker;
		ColorRGBA m_color;
		ArraySimple<ColorRGBA> m_color_presets;
		unsigned int m_id_active;
	};

#endif
