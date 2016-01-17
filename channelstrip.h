#ifdef __WAND__
target[name[channelstrip.h] type[include]]
dependency[channelstrip.o]
#endif

#ifndef CHANNELSTRIP_H
#define CHANNELSTRIP_H

#include "framework/widget.h"
#include "framework/valueinput.h"

class ChannelData;
class GuiContainer;
class BoxVertical;
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

		class ValueInputHandler:public ValueInput::EventHandler
			{
			public:
				ValueInputHandler(ChannelStrip& strip):r_strip(strip)
					{}

				double valueMap(ValueInput& source,double x) const noexcept;
				double valueMapInverse(ValueInput& source,double y) const noexcept;

			private:
				ChannelStrip& r_strip;
			} m_input_handler;

		ChannelData* r_channel;
		BoxVertical* m_box;
			Textbox* m_label;
			Knob* m_fadetime;
			Slider* m_level;
	};

#endif
