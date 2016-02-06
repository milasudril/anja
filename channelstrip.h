#ifdef __WAND__
target[name[channelstrip.h] type[include]]
dependency[channelstrip.o]
#endif

#ifndef CHANNELSTRIP_H
#define CHANNELSTRIP_H

#include "framework/widget.h"

class ChannelData;
class Knob;
class Slider;
class BoxVertical;
class Window;
class Textbox;
class ChannelStripHandler;
class GuiContainer;
class ColorView;

class ChannelStrip:public Widget
	{
	public:
		static ChannelStrip* create(GuiContainer& parent,ChannelStripHandler& handler
			,unsigned int id);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void channelDataSet(const ChannelData& channel);

		unsigned int idGet() const
			{return m_id;}

	private:
		ChannelStrip(GuiContainer& parent,ChannelStripHandler& handler
			,unsigned int id);
		~ChannelStrip();

		BoxVertical* m_box;
			Textbox* m_label;
			ColorView* m_color;
			Knob* m_fadetime;
			Slider* m_level;

		unsigned int m_id;
	};

#endif
