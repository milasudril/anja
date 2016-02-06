#ifdef __WAND__
target[name[sessionview.h] type[include]]
dependency[sessionview.o]
#endif

#ifndef SESSIONVIEW_H
#define SESSIONVIEW_H

#include "framework/widget.h"
#include "waveformrangeview.h"
#include "waveformdataview.h"
#include "sessiondataview.h"
#include "channelstriphandler.h"
#include "framework/keyboardview.h"

class GuiContainer;
class Session;
class BoxVertical;
class BoxHorizontal;
class TabView;
class ChannelView;
class SessionControl;
class Delimiter;

class SessionView:public Widget
	{
	public:
		static SessionView* create(GuiContainer& parent,Session& session
			,KeyboardView::EventHandler& keyboard_input
 			,WaveformDataView::EventHandler& data_eventhandler
			,WaveformRangeView::EventHandler& rangeview_handler
			,ChannelStripHandler::EventHandler& channelstrip
			,SessionDataView::EventHandler& sessiondata_handler);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void slotDisplay(uint8_t slot);

		void channelDisplay(unsigned int channel);

		void sessionSet(Session& session);

		void channelNameUpdate(unsigned int channel);

		void channelColorUpdate(unsigned int channel);

		void sessionTitleUpdate();

		void keyboardViewUpdate();

		void fullscreenToggle();

		bool fullscreenIs() const
			{return m_fullscreen_state;}

	private:
		SessionView(GuiContainer& parent,Session& session
			,KeyboardView::EventHandler& keyboard_input
			,WaveformDataView::EventHandler& data_eventhandler
			,WaveformRangeView::EventHandler& rangeview_handler
			,ChannelStripHandler::EventHandler& channelstrip
			,SessionDataView::EventHandler& sessiondata_handler);
		~SessionView();
		GuiContainer& r_parent;
		Session* r_session;
		bool m_fullscreen_state;

		BoxHorizontal* m_box;
			SessionControl* m_control;
			Delimiter* m_delimiter;
			BoxVertical* m_vbox;
				KeyboardView* m_keyboard;
				TabView* m_tabs;
					WaveformDataView* m_dataview;
					ChannelView* m_mixer;
					SessionDataView* m_sessiondata;
	};

#endif
