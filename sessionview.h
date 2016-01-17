#ifdef __WAND__
target[name[sessionview.h] type[include]]
dependency[sessionview.o]
#endif

#ifndef SESSIONVIEW_H
#define SESSIONVIEW_H

#include "framework/widget.h"
#include "waveformrangeview.h"
#include "waveformdataview.h"
#include "framework/keyboardview.h"

class GuiContainer;
class Session;
class BoxVertical;
class TabView;
class Mixer;

class SessionView:public Widget
	{
	public:
		static SessionView* create(GuiContainer& parent,Session& session
			,KeyboardView::EventHandler& keyboard_input
 			,WaveformDataView::EventHandler& data_eventhandler
			,WaveformRangeView::EventHandler& rangeview_handler);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void slotDisplay(uint8_t slot);

		void sessionSet(Session& session);

		void keyboardViewUpdate();

	private:
		SessionView(GuiContainer& parent,Session& session
			,KeyboardView::EventHandler& keyboard_input
			,WaveformDataView::EventHandler& data_eventhandler
			,WaveformRangeView::EventHandler& rangeview_handler);
		~SessionView();

		Session* r_session;
		BoxVertical* m_box;
			KeyboardView* m_keyboard;
			TabView* m_tabs;
				WaveformDataView* m_dataview;
				Mixer* m_mixer;
	};

#endif
