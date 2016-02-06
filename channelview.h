#ifdef __WAND__
target[name[channelview.h] type[include]]
dependency[channelview.o]
#endif

#ifndef CHANNELVIEW_H
#define CHANNELVIEW_H

#include "framework/widget.h"
#include "channelstrip.h"
#include "channelstriphandler.h"
#include "framework/array_simple.h"

class GuiContainer;
class BoxHorizontal;
class ChannelData;
class ScrollWindow;
class Slider;

class ChannelView:public Widget
	{
	public:
		static ChannelView* create(GuiContainer& parent
			,ChannelStripHandler::EventHandler& handler,const ChannelData* channels
			,unsigned int n_channels
			,ColorRGBA* color_presets,size_t n_presets);

		void destroy();

		void channelDataSet(const ChannelData& data,unsigned int channel);

		void channelDataSet(const ChannelData* channels,unsigned int n_channels);

		const GuiHandle& handleNativeGet() const;

	private:
		ChannelView(GuiContainer& parent,ChannelStripHandler::EventHandler& handler
			,const ChannelData* channels,unsigned int n_channels
			,ColorRGBA* color_presets,size_t n_presets);
		~ChannelView();
		BoxHorizontal* m_box;
		ScrollWindow* m_scroll;
		BoxHorizontal* m_box_channels;
		ArraySimple<ChannelStrip*> m_strips;
		Slider* m_master_gain;
		ChannelStripHandler m_handler;
	};

#endif

