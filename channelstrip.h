#ifdef __WAND__
target[name[channelstrip.h] type[include]]
dependency[channelstrip.o]
#endif

#ifndef CHANNELSTRIP_H
#define CHANNELSTRIP_H

#include "framework/widget.h"
#include "framework/valueinput.h"
#include "framework/boxvertical.h"

class ChannelData;
class Textbox;
class Knob;
class Slider;

class ChannelStrip:public Widget
	{
	public:
		static ChannelStrip* create(GuiContainer& parent);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void channelDataSet(ChannelData& channel)
			{
			r_channel=&channel;
			update();
			}

		void update();

	private:
		ChannelStrip(GuiContainer& parent);
		~ChannelStrip();

		friend class ValueInputHandler;

		class ValueInputHandler:public ValueInput::EventHandler
			,public BoxVertical::EventHandler
			{
			public:
				ValueInputHandler(ChannelStrip& strip):r_strip(strip)
					{}

				void textGet(ValueInput& source,double value,TextBuffer& buffer);
				double valueGet(ValueInput& source,const char* text);
				double valueMap(ValueInput& source,double x) const noexcept;
				double valueMapInverse(ValueInput& source,double y) const noexcept;

				void onCommand(BoxVertical& source,unsigned int id);

			private:
				ChannelStrip& r_strip;
			} m_input_handler;

		ChannelData* r_channel;
		BoxVertical* m_box;
			Textbox* m_label;
			Knob* m_fadetime;
			Slider* m_level;

		void gainSet(double gain);
		void fadeTimeSet(double time);
	};

#endif
