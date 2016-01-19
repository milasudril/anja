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
class TabView;
class ChannelView;

class SessionView:public Widget
	{
	public:
		static SessionView* create(GuiContainer& parent,Session& session
			,KeyboardView::EventHandler& keyboard_input
 			,WaveformDataView::EventHandler& data_eventhandler
			,WaveformRangeView::EventHandler& rangeview_handler
			,ChannelStrip::EventHandler& channelstrip);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void slotDisplay(uint8_t slot);

		void sessionSet(Session& session);

		void keyboardViewUpdate();

	private:
		SessionView(GuiContainer& parent,Session& session
			,KeyboardView::EventHandler& keyboard_input
			,WaveformDataView::EventHandler& data_eventhandler
			,WaveformRangeView::EventHandler& rangeview_handler
			,ChannelStrip::EventHandler& channelstri);
		~SessionView();

		Session* r_session;
		BoxVertical* m_box;
			KeyboardView* m_keyboard;
			TabView* m_tabs;
				WaveformDataView* m_dataview;
				ChannelView* m_mixer;
	};

#endif
