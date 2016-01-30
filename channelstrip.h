#ifdef __WAND__
target[name[channelstrip.h] type[include]]
dependency[channelstrip.o]
#endif

#ifndef CHANNELSTRIP_H
#define CHANNELSTRIP_H

#include "framework/widget.h"
#include "framework/valueinput.h"
#include "framework/textbox.h"
#include "framework/colorview.h"
#include "framework/colorpicker.h"
#include "framework/color.h"
#include "framework/array_simple.h"

class ChannelData;
class Knob;
class Slider;
class BoxVertical;
class Window;

class ChannelStrip:public Widget
	{
	public:
		class EventHandler
			{
			public:
				virtual void onLabelChange(ChannelStrip& source,const char* label)=0;
				virtual void onFadeTimeChange(ChannelStrip& source,float time)=0;
				virtual void onGainChange(ChannelStrip& source,float value)=0;
				virtual void onColorChange(ChannelStrip& sorce,const ColorRGBA& value)=0;
			};

		void doLabelChange(const char* label)
			{r_handler->onLabelChange(*this,label);}

		void doFadeTimeChange(float time)
			{r_handler->onFadeTimeChange(*this,time);}

		void doGainChange(float value)
			{r_handler->onGainChange(*this,value);}

		void doColorChange(const ColorRGBA& color)
			{r_handler->onColorChange(*this,color);}

		static ChannelStrip* create(GuiContainer& parent,EventHandler& handler
			,unsigned int id);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void channelDataSet(const ChannelData& channel);

		unsigned int idGet() const
			{return m_id;}

	private:
		ChannelStrip(GuiContainer& parent,EventHandler& handler
			,unsigned int id);
		~ChannelStrip();

		EventHandler* r_handler;

		friend class ValueInputHandler;

		class ValueInputHandler:public Textbox::EventHandler
			,public ColorView::EventHandler,public ValueInput::EventHandler
			,public ColorPicker::EventHandler
			{
			public:
				ValueInputHandler(ChannelStrip& strip);
				~ValueInputHandler();

				void textGet(ValueInput& source,double value,TextBuffer& buffer);
				double valueGet(ValueInput& source,const char* text);
				double valueMap(ValueInput& source,double x) const noexcept;
				double valueMapInverse(ValueInput& source,double y) const noexcept;

				void onLeave(Textbox& source);

				void onActionPerform(ColorView& source);

				void onConfirmed(ColorPicker::Tag x);

			private:
				ChannelStrip& r_strip;
				Window* m_colordlg;
				ColorPicker* m_picker;
				ColorRGBA m_color;
				ArraySimple<ColorRGBA> m_color_presets;
			} m_input_handler;

		BoxVertical* m_box;
			Textbox* m_label;
			ColorView* m_color;
			Knob* m_fadetime;
			Slider* m_level;

		unsigned int m_id;
	};

#endif
