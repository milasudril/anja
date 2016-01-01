#ifdef __WAND__
target[name[sessionview.h] type[include]]
dependency[sessionview.o]
#endif

#ifndef SESSIONVIEW_H
#define SESSIONVIEW_H

#include "framework/widget.h"
#include "waveformrangetrimmer.h"
#include "waveformdataview.h"
#include "framework/keyboardlayout.h"

class GuiContainer;
class Session;
class KeyboardView;
class KeyboardController;
class WaveformDataView;
class BoxVertical;
class AudioConnection;

class SessionView:public Widget
	{
	public:
		static SessionView* create(GuiContainer& parent,Session& session
			,KeyboardController& keyboard_input
			,WaveformRangeView::EventHandler& rangeview_handler);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void slotDisplayFromScancode(uint8_t scancode);

		void slotDisplayFromSlotIndex(uint8_t slot_index);

		void sessionSet(Session& session);

		void keyCurrentLabelSet(const char* label);

		void keyCurrentColorSet(const ColorRGBA& color_new);

		void keyCurrentColorSet(const char* colorstr);

	private:
		class WaveformDataEventHandler:public WaveformDataView::EventHandler
			{
			public:
				WaveformDataEventHandler(SessionView& view):r_view(&view){}
				void onSourceChange(WaveformDataView& source
					,const char* filename_new);
				void onDescriptionChange(WaveformDataView& source
					,const char* description_new);
				void onColorChange(WaveformDataView& source
					,const ColorRGBA& color_new);
				void onColorChange(WaveformDataView& source
					,const char* colorstr);
			private:
				SessionView* r_view;
			};

		SessionView(GuiContainer& parent,Session& session
			,KeyboardController& keyboard_input
			,WaveformRangeView::EventHandler& rangeview_handler);
		~SessionView();

		Session* r_session;
		BoxVertical* m_box;
			KeyboardView* m_keyboard;
			WaveformDataView* m_dataview;

		WaveformDataEventHandler m_waveformevents;
		KeyboardLayout::KeyDescriptor* r_key_current;
	};

#endif
