#ifdef __WAND__
target[name[sessionview.h] type[include]]
dependency[sessionview.o]
#endif

#ifndef SESSIONVIEW_H
#define SESSIONVIEW_H

#include "framework/widget.h"
#include "waveformrangeview.h"
#include "waveformdataview.h"
#include "channelstrip.h"
#include "framework/keyboardview.h"

class GuiContainer;
class Session;
class BoxVertical;
class BoxHorizontal;
class TabView;
class ChannelView;
class SessionControl;
class TitleView;
class SessionDataView;

class SessionView:public Widget
	{
	public:
		static SessionView* create(GuiContainer& parent,Session& session
			,TitleView& title_view
			,KeyboardView::EventHandler& keyboard_input
 			,WaveformDataView::EventHandler& data_eventhandler
			,WaveformRangeView::EventHandler& rangeview_handler
			,ChannelStrip::EventHandler& channelstrip);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void slotDisplay(uint8_t slot);

		void channelDisplay(unsigned int channel);

		void sessionSet(Session& session);

		void channelNameUpdate(unsigned int channel);

		void channelColorUpdate(unsigned int channel);

		void keyboardViewUpdate();

		void fullscreenToggle();

		bool fullscreenIs() const
			{return m_fullscreen_state;}

	private:
		SessionView(GuiContainer& parent,Session& session
			,TitleView& title_view
			,KeyboardView::EventHandler& keyboard_input
			,WaveformDataView::EventHandler& data_eventhandler
			,WaveformRangeView::EventHandler& rangeview_handler
			,ChannelStrip::EventHandler& channelstri);
		~SessionView();
		TitleView& r_tw;
		Session* r_session;
		bool m_fullscreen_state;

		BoxVertical* m_box;
			SessionControl* m_control;
			KeyboardView* m_keyboard;
			TabView* m_tabs;
				WaveformDataView* m_dataview;
				ChannelView* m_mixer;
				SessionDataView* m_sessiondata;
	};

#endif
