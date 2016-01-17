#ifdef __WAND__
target[name[channelstrip.h] type[include]]
#endif

#ifndef CHANNELSTRIP_H
#define CHANNELSTRIP_H

#include "framework/widget.h"

class BoxVertical;
class Textbox;
class Knob;

class ChannelStrip:public Widget
	{
	public:
	private:
		ChannelData* r_channel;
		BoxVertical* m_box;
			Textbox* m_label;
			Label* m_decaytime_label;
			Textbox* m_decaytime_input;
			Slider* m_slider;
	};

#endif
