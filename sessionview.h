#ifdef __WAND__
target[name[sessionview.h] type[include]]
dependency[sessionview.o]
#endif

#ifndef SESSIONVIEW_H
#define SESSIONVIEW_H

#include "framework/widget.h"
#include "waveformrangetrimmer.h"
#include "framework/keyboardview.h"
#include "framework/keyboardlayout.h"

class GuiContainer;
class Session;
class KeyboardView;
class WaveformDataView;
class BoxVertical;

class SessionView:public Widget
	{
	public:
		static SessionView* create(GuiContainer& parent,Session& session);

		void destroy();

		const GuiHandle& handleNativeGet() const;

		void slotDisplayFromScancode(uint8_t scancode);

		void slotDisplayFromSlotIndex(uint8_t slot_index);

		void sessionSet(Session& session);

	private:
		class KeyboardEventHandler:public KeyboardView::EventHandler
			{
			public:
				KeyboardEventHandler(SessionView& view):r_view(&view){}

				void onMouseUp(KeyboardView& source,uint8_t scancode
					,keymask_t key_mask)
					{
					r_view->slotDisplayFromScancode(scancode);
					source.update();
					}

				void onKeyDown(KeyboardView& source,uint8_t scancode)
					{}

			private:
				SessionView* r_view;
			};

		SessionView(GuiContainer& parent,Session& session);
		~SessionView();

		Session* r_session;
		BoxVertical* m_box;
			KeyboardView* m_keyboard;
			WaveformDataView* m_dataview;

		WaveformRangeTrimmer m_trimmer;
		KeyboardEventHandler m_keyboardevents;
		KeyboardLayout::KeyDescriptor* r_key_current;
	};

#endif
